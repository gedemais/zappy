
TERM_CLIENT=/dev/pts/3
TERM_SERVER=/dev/pts/4
(sleep 3 && ~/zappy_save/client/client -p 8080 -n foo) 2> ${TERM_CLIENT}  & /home/lmariott/zappy_1406/server/server -p 8080 -x 30 -y 30 -n foo -c 3 -t 200 > ${TERM_SERVER}

