#!/bin/bash


<<<<<<< HEAD
arr=("a" "b" "c" "d" "e")
		# "aa" "ab" "ac" "ad" "ae" "af" "ag" "ah" "ai" "aj" "ak" "al" "am" "an" "ao" "ap" "aq" "ar" "as" "at" "au" "av" "aw" "ax" "ay" "az")
=======
arr=("a" "b" "c" "d" "e" "f" "g" "h" "i" "j" "k" "l" "m" "n" "o" "p")
>>>>>>> main

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
<<<<<<< HEAD
	python3 main.py -p 8080 -n "${arr[i]}" -a localhost &
=======
	touch logs/"${arr[i]}".log
	chmod 777 logs/"${arr[i]}".log
	python3 main.py -p 8080 -n "${arr[i]}" -a localhost > logs/"${arr[i]}".log &
>>>>>>> main
	sleep 0.33
done
