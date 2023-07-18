import socket
import json
from time import sleep

host = socket.gethostname()
port = 8080                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))


print('connected')
reply = s.recv(1024).decode('utf-8')

print(reply)

print('sending team_name...')
s.send(bytes('team_a'.encode('utf-8')))
print('sent')

reply = s.recv(1024).decode('utf-8')

print(reply)

i = 0

while True:
    if i % 30 == 0:
        s.send(bytes('gauche'.encode('utf-8')))
    elif i % 6 == 0:
        s.send(bytes('droite'.encode('utf-8')))
    else:
        s.send(bytes('avance'.encode('utf-8')))
        
    reply = s.recv(1024).decode('utf-8')

    s.send(bytes('voir'.encode('utf-8')))

    reply = s.recv(1024).decode('utf-8')

    reply = reply[1:]
    reply = reply[:len(reply) - 2]
    reply = reply.split(',')[0]
    reply = reply.split(' ')
    while 'nourriture' in reply:
        s.send(bytes('prend nourriture'.encode('utf-8')))
        b_reply = s.recv(1024).decode('utf-8')
        reply.remove('nourriture')

    print(reply)
    #while 'nourriture'
    print('-' * 80)
    #tiles = reply.spl

    #print(see)
    
    i += 1

s.close()
