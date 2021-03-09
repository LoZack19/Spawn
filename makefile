CC = /usr/bin/gcc

spawn: main.c
	$(CC) main.c -o spawn

install:
	sudo install spawn /usr/bin
	rm spawn

# Run as root
uninstall:
	rm /usr/bin/spawn