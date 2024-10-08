#! /bin/bash

read -p "Enter number for factorial calculation: " number

factorial=1

while [ $number -gt 0 ]
do
  factorial=$(( factorial*number ))
  number=$(( number-1 ))
done

echo "Factorial of $number is $factorial "