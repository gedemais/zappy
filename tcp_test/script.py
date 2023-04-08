import socket
from time import sleep

instructions = [
    b"avance\n",
    b"droite\n"
]

tot = 100

host = socket.gethostname()
port = 8080                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

for t in range(tot):
    i = 0
    while i < len(instructions):

        s.sendall(instructions[i])

        print(instructions[i])
        sleep(0.1)
        i += 1

s.close()
