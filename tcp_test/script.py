import socket
from time import sleep

instructions = [
    b"avance\n",
    b"avance\n",
    b"avance\n",
    b"droite\n",
    b"avance\n",
    b"avance\n",
    b"avance\n",
    b"droite\n",
    b"avance\n",
    b"avance\n",
    b"avance\n",
    b"droite\n",
    b"avance\n",
    b"avance\n",
    b"avance\n",
    b"droite\n"
]

host = socket.gethostname()
port = 8080                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

i = 0
while i < len(instructions):

    s.sendall(instructions[i])

    print(instructions[i])
    sleep(0.5)
    i += 1

    if i == len(instructions):
        i = 0

s.close()
