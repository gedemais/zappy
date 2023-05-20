#!/bin/bash
for i in {1..32}
do
	python3 tcp_test/client_v0.py &
done
