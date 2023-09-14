#!/bin/bash


arr=("a" "b" "c" "d")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	python3 main.py -p 8080 -n "${arr[i]}" -a localhost &
	sleep 0.33
done
