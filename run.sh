#!/usr/bin/bash

rm -f log

# VALGRIND=valgrind

# note: type "tty" in 2 terminals and put it here
TERM_CLIENT=/dev/pts/1
TERM_SERVER=/dev/pts/2

(sleep 3 && ${VALGRIND} ./zappy_client -p 8080 -n foo -i 6) 2>&1 | tee -a log > ${TERM_CLIENT}  & ./zappy_server -p 8080 -x 30 -y 30 -n foo -c 6 -t 100 > ${TERM_SERVER}
