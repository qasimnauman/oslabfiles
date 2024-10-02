#! /bin/bash

filename=$1

if ! [[ -f $filename ]]
    then touch $filename

    tee $filename <<EOF
    The contents in this file are written using the tee paired with end of file
    to write the contents defined in the EOF block to the end of file.
    This ensures multiple lines being written to the file at the same time.
EOF

else
    echo "$filename already exists"
fi