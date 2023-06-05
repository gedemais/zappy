#! /bin/sh


# docker run -it -p 8080:8080 -e ZHOST="127.0.0.1" -e ZPORT="8080" -e ZTEAM="foo" zappy_client
# ./client -h 127.0.0.1 -p 8080 -n foo
/zappy/client/client "-h" $ZHOST "-p" $ZPORT "-n" $ZTEAM
