FROM debian:latest

RUN apt-get update -y \
	&& apt-get upgrade -y \
	&& apt-get install -y build-essential

EXPOSE 8080

ENV TERM "xterm-256color"

COPY . /zappy_server/

WORKDIR /zappy_server/server

RUN make
