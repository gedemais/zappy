# Zappy

Zappy is an automatic video game where AIs play with each others.
AI players are divided into teams of 6 members. The winning team
is the one that will have its 6 players reach the maximum level first.
This project is divided in two parts :

# Server
The server is a program developped in C. It will manage the map, resources, and population of the world
by communicating with the clients and enforcing the game's rules, updating world's data at each time interval.

## Server's features :
- [ ] World map emulation
- [ ] Player implementation
- [ ] Command's procedures implementation
- [ ] Commands transmission via TCP/IP

# Client
The client program will pilot a player by sending messages to the server via TCP.
