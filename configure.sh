#!/bin/bash

gcc main.c -o spawn &
echo "Compiling..."

spawn="spawn.sh"

printf "#" > $spawn
printf "!/bin/bash\nif ! spawn \$1 2>/dev/null\nthen\n\t./spawn \$1 2>/dev/null\nfi" >> $spawn

chmod u+x $spawn

echo "spawn.sh generated succesfully"