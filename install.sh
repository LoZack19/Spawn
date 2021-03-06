#!/bin/bash

FILE="spawn"

if test ! -f "$FILE"
then
    echo "Running configure.sh"
    ./configure.sh
fi

echo "Configuration completed"

var=0

printf "Are you sure you want to install spawn [Y/n] "
read var;
if [ $var == "Y" ]
then
    sudo install spawn /usr/bin

    if test ! -f /usr/bin/spawn
    then
        echo "[./install.sh]: Wrong password!"
        exit
    fi
    
    printf "\n"
    echo "Installation completed"
    echo "To remove the program use the following command:"
    echo "$ sudo rm /usr/bin/spawn"
else
    echo "Installation aboorted"
fi

exit
