#! /bin/bash

i=0
until [[ i -eq 100 ]];
do
 echo "$i"
 i=$((i+1))
done
