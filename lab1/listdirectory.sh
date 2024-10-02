#! /bin/bash

# default directory is the one in which the file is present
# if arguemnt not specified then it will use the default directory to list the files

directory=${1:-.}

# listing contents of directory
ls -l $directory