import socket
from gameBoard import GameBoard

host = socket.gethostname()
port = 8080                   # The same port as used by the server
init = 'GRAPHIC\n' 

class ServerComm(GameBoard):
    def __init__(self):
        super.__init__(self)
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((host, port))
        self.reply = self.s.recv(1024).decode('utf-8')
        print(self.reply) # BIENVENUE
        self.s.send(bytes(init.encode('utf-8')))
        self.reply = self.s.recv(1024).decode('utf-8')
        print(self.reply)
            
    def unknown_command():
        print("Unknown command.")

    def bad_parameters():
        print("Bad parameters for the command.")
