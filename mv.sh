#!/bin/bash

list=`ls`
if [[ ! -d "execrise" ]]
then
		mkdir execrise
fi

for file in $list
do
	if [[ "$file" =~ ^[0-9].* ]]
	then
			echo $file
			mv $file execrise
	fi
done
		
