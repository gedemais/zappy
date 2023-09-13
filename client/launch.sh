#!/bin/bash


arr=("a" "b" "c" "d" "e" "f" "g" "h" "i" "j" "k" "l" "m" "n" "o" "p" "q" "r" "s" "t" "u" "v" "w" "x" "y" "z"\
		"aa" "ab" "ac" "ad" "ae" "af" "ag" "ah" "ai" "aj")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	sudo touch logs/"${arr[i]}".log
	sudo chmod 777 logs/"${arr[i]}".log
	sudo python3 main.py -p 8080 -n "${arr[i]}" -a localhost > logs/"${arr[i]}".log &
	sleep 0.33
done
