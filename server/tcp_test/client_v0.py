import socket
from time import sleep

host = socket.gethostname()
port = 8080                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

moves = ['avance\n',  'prend linemate\n', 'avance\n', 'prend linemate\n', 'avance\n', 'prend linemate\n', 'avance\n', 'prend linemate\n', 'voir\n', 'inventaire\n', 'incantation\n']

cmd_index = 0

print('connected')
reply = s.recv(1024).decode('utf-8')

print(reply)

print('sending team_name...')
s.send(bytes('foo'.encode('utf-8')))
print('sent')

reply = s.recv(1024).decode('utf-8')

print(reply)

while True:
    for i in range(len(moves)):
        reply = ""
        s.send(bytes(moves[i].encode('utf-8')))
        reply = s.recv(1024).decode('utf-8')

s.close()
