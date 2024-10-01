#!/bin/bash

# Assigning inputs to arguments
dirname=$1
startno=$2
endno=$3

# Displays error when no argument is provided
if [ -z "$dirname" ] || [ -z "$startno" ] || [ -z "$endno" ]; then
    echo "No Argument provided"
    echo "Argument order: dirname startno endno"
    exit 1
fi

# Checking if both start and end no are integers or not
if ! [[ "$startno" =~ ^[0-9]+$ ]] || ! [[ "$endno" =~ ^[0-9]+$ ]]; then
    echo "Error: startno and endno must be integers."
    exit 1
fi

# Create directories in the specified range
for (( i=startno; i<=endno; i++ ))
do
    mkdir "${dirname}${i}"
done
