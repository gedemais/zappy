#!/bin/bash


arr=("a" "b" "c" "d" "e" "f" "g" "h" "i" "j" "k" "l" "m" "n" "o" "p" "q" "r" "s" "t" "u" "v" "w" "x" "y" "z")
		# "aa" "ab" "ac" "ad" "ae" "af" "ag" "ah" "ai" "aj" "ak" "al" "am" "an" "ao" "ap" "aq" "ar" "as" "at" "au" "av" "aw" "ax" "ay" "az")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	sudo python3 main.py -p 8080 -n "${arr[i]}" -a localhost &
	sleep 0.33
done
