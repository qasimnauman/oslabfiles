#!/bin/bash

var1=$1
var2=$2
opr=$3
check="y"

if [ -z "$var1" ] || [ -z "$var2" ] || [ -z "$opr" ]; then
    echo "No Argument provided"
    echo "Argument order: number1 number2 operator(+,-,/,*,%)"
    exit 1
fi

if ! [[ "$var1" =~ ^[0-9]+$ ]] || ! [[ "$var2" =~ ^[0-9]+$ ]]; then
    echo "Error: number 1 and number 2 must be integers"
    exit 1
fi

until [ "$check" != "y" ]; do
    if [[ "$opr" == "+" ]]
    then
        result=$((var1 + var2))
    elif [[ "$opr" == "-" ]]
    then
        result=$((var1 - var2))
    elif [[ "$opr" == "*" ]]
    then
        result=$((var1 * var2))
    elif [[ "$opr" == "/" ]]
    then
        if [[ $var2 -le 0 ]]
        then
            echo "Denominator cannot be less than or zero"
            result=0
        else
            result=$((var1 / var2))
        fi
    elif [[ "$opr" == "%" ]]
    then
        result=$((var1 % var2))
    else
        echo "Error: Invalid operator. Use +, -, *, /, or %"
        exit 1
    fi

    echo "Result: $result"

    read -p "Do you want to perform another operation (y/n)? " check
    if [[ "$check" == "y" ]]
    then
        read -p "Enter number 1: " var1
        read -p "Enter numebr 2: " var2
        read -p "Enter operation (+,-,/,*,%): " opr
    else
        exit 1
    fi
done