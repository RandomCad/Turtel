#! /bin/bash

./tests 9 9


# Check the exit status of the program
if [ $? -eq 1 ]; then
    exit 0
else
    echo "The errors arn't correctly detected."
    exit 1
fi
