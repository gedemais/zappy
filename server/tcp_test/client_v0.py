import socket
from time import sleep

host = socket.gethostname()
port = 8080                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

moves = ['avance\n', 'droite\n', 'broadcast bonjour\n']

print('connected')
reply = s.recv(1024).decode('utf-8')

print(reply)

print('sending team_name...')
s.send(bytes('team_a'.encode('utf-8')))
print('sent')

reply = s.recv(1024).decode('utf-8')

print(reply)

while True:
    for i in range(len(moves)):
        s.send(bytes(moves[i].encode('utf-8')))
        reply = s.recv(1024).decode('utf-8')
        print(reply)

s.close()
