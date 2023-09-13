#!/bin/bash


arr=("a" "b" "c" "d" "e")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	sudo touch logs/"${arr[i]}".log
	sudo chmod 777 logs/"${arr[i]}".log
	sudo python3 main.py -p 8080 -n "${arr[i]}" -a localhost > logs/"${arr[i]}".log &
	sleep 0.33
done
