#!/bin/bash


arr=("a" "b" "c" "d" "e" "f" "g" "h" "i" "j" "k" "l" "m" "n" "o" "p")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	touch logs/"${arr[i]}".log
	chmod 777 logs/"${arr[i]}".log
	python3 main.py -p 8080 -n "${arr[i]}" -a localhost > logs/"${arr[i]}".log &
	sleep 0.33
done
