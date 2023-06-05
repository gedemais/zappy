#! /bin/sh


# docker run -it -p 8080:8080 -e ZPORT="8080" -e ZX="10" -e ZY="10" -e ZTEAM="foo" -e ZCLIENT="10" -e ZT="25" zappy_server
# ./server -p 8080 -x 10 -y 10 -n foo -c 10 -t 25
/zappy/server/server "-p" $ZPORT "-x" $ZX "-y" $ZY "-n" $ZTEAM "-c" $ZCLIENT "-t" $ZT
