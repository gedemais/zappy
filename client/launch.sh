#!/bin/bash


arr=("a" "b" "c" "d" "e")
		# "aa" "ab" "ac" "ad" "ae" "af" "ag" "ah" "ai" "aj" "ak" "al" "am" "an" "ao" "ap" "aq" "ar" "as" "at" "au" "av" "aw" "ax" "ay" "az")

for ((i = 0; i < ${#arr[@]}; i++))
do
	echo "${arr[i]}"
	python3 main.py -p 8080 -n "${arr[i]}" -a localhost &
	sleep 0.33
done
