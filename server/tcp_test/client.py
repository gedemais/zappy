import socket

host = socket.gethostname()
port = 8080                   # The same port as used by the server
team_name = 'team_a' 

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

moves = ['avance\n', 'avance\n', 'avance\n', 'avance\n', 'droite\n']

reply = s.recv(1024).decode('utf-8')
print(reply) # BIENVENUE

s.send(bytes(team_name.encode('utf-8')))

reply = s.recv(1024).decode('utf-8')

print(reply)

while True:
    for i in range(len(moves)):
        s.send(bytes(moves[i].encode('utf-8')))
        reply = s.recv(1024).decode('utf-8')
        print(reply) # Server's response to last command
