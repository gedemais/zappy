import socket
from time import sleep

host = socket.gethostname()
port = 8080                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#s.settimeout()
s.connect((host, port))

reply = s.recv(1024).decode('utf-8')

print(reply)

s.send(bytes('team_a'.encode('utf-8')))

reply = s.recv(1024).decode('utf-8')

print(reply)

s.close()
