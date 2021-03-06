#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>


#define	PATH_SIZE	200
#define FIFO_PATH	"/tmp/spawn_fifo"

int streq(char* str1, char* str2, int size)
{
	for(int i = 0; i < size; i++)
		if(str1[i] != str2[i])
			return 0;
	return 1;
}

void helplist()
{
	printf("Use the following syntax:\n"
		  "spawn [PROGRAM] [ARGS]\n");
}

char* getFullPath(char* path)
{
	pid_t pid;
	int status;
	char* wholepath = (char*) malloc(PATH_SIZE);
	int fifo_fd;

	if(!~mkfifo(FIFO_PATH, 0666))
		perror("Failed to create fifo");
	
	pid = fork();
	if(pid < 0) // failed
	{
		fputs("Failed to fork the process. Try again!\n", stderr);
		exit(-1);
	}
	if(!pid) //child
	{
		char* args[] = {"which", path};
		fifo_fd = open(FIFO_PATH, O_WRONLY);
		if(!~fifo_fd)
		{
			perror("Failed to open fifo in write only mode");
			exit(-1);
		}
		dup2(fifo_fd, 1);
		close(fifo_fd);

		sleep(1);

		if(!~execv("/usr/bin/which", args))
		{
			fprintf(stderr, "Failed to execute process: %s\n", strerror(errno));
			return NULL;
		}
	}

	fifo_fd = open(FIFO_PATH, O_RDONLY);
	if(!~fifo_fd)
	{
		perror("Failed to open fifo in read only mode");
		return NULL;
	}
	status = PATH_SIZE;

	if(!~read(fifo_fd, wholepath, PATH_SIZE))
	{
		perror("Couldn't read from fifo");
		return NULL;
	}
	status = strlen(wholepath);
	wholepath[status-1] = 0;

	wholepath = realloc(wholepath, status);
	unlink(FIFO_PATH);
	wait(NULL);
	return wholepath;
}

int main(int argc, char* argv[])
{
	pid_t pid_check = 0;
	int status = 0;
	char* wholepath;

	if(argc == 1)
	{
		fputs("Too few arguments. Use -h or --help to display the help list\n", stderr);
		exit(-1);
	}
	else if(streq("-h", argv[1], 3) || streq("--help", argv[1], 6))
	{
		helplist();
		exit(0);
	}
	
	// Find the path of the program
	if(argv[1][0] == '/')
		wholepath = argv[1];
	else if(streq(argv[1], "./", 2))
	{
		wholepath = getcwd(wholepath, PATH_SIZE);
		if(!wholepath)
		{
			perror("Failed to expand the full path");
			_exit(-1);
		}
		sprintf(wholepath, "%s%s", wholepath, &argv[1][1]);
	}
	else
		wholepath = getFullPath(argv[1]);

	pid_check = fork();
	if(pid_check < 0) // error
	{
		fputs("Failed to fork the process. Try again!\n", stderr);
		exit(-1);
	}
	else if(!pid_check) // child
	{
		
		if(!~setsid())
		{
			perror("Failed to detach child from parent");
			exit(-1);
		}
		
		if(!~execv(wholepath, &argv[1]))
		{
			perror("Failed to execute process");
			exit(-1);
		}
		// New process spawned
	}

	return 0;
}
