#!/bin/bash


arr=("a" "b" "c" "d") #"e" "f" "g" "h" "i" "j" "k" "l" "m" "n" "o" "p")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	sudo touch "${arr[i]}".log
	sudo chmod 777 "${arr[i]}".log
	sudo python3 main.py -p 8080 -n "${arr[i]}" -a localhost > "${arr[i]}".log &
	sleep 0.33
done
