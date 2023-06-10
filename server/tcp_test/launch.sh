#!/bin/bash
for i in {1..3}
do
	python3 tcp_test/client_v0.py &
	sleep 1
done
