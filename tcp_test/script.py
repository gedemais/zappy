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
s.settimeout(0.5)
s.connect((host, port))

al = 0

for t in range(tot):
    i = 0
    while i < len(instructions):

        s.sendall(instructions[i])
        try:
            reply = s.recv(1024).decode('utf-8')
        except:
            reply = 'empty'
            print('no reply')
            pass

        if reply != 'empty':
            print(reply)

        print(instructions[i])
        print(al)
        sleep(0.1)
        i += 1
        al += 1

s.close()
