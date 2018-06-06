#!/bin/bash

varlist=$(ls -l ../lib/*.c)

for var in $(ls  ../lib/*.c)
do
	IFS='\/'
	count=0
	for file in $var
	do
		if [ $count -eq 2 ]
		then
			tmp=$(echo \#include \"$file\")
			echo $tmp
		fi
		count=$[ $count + 1 ]
	done
done
