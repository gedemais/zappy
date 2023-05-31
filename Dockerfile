FROM debian:latest

RUN apt-get update \
	&& apt-get upgrade \
	&& apt-get install -y build-essential

ENV SERVER_ARGS=""

WORKDIR /zappy_server/

COPY ./server/ /zappy_server/

EXPOSE 8080

RUN make

# CMD ["./server", "-p 8080 -x 10 -y 10 -n foo -c 10 -t 25"]
