import socket

class   Connector():

    def __init__(self, host, port, timeout=60):
        # Socket initialization
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Socket connection
        self.socket.connect((host, port))
        # Timeout setting to avoid recv from receiving all the time
        self.socket.settimeout(1.0 / timeout)


    def authenticate(self):
        self.send('GRAPHICAL\n')

        response = None
        while response == None:
            response = self.receive()
        print(response)

        if response != 'BIENVENUE\n':
            print('onepolesbienvenus')
            exit(1)

        response = None
        while response == None:
            response = self.receive()

        return response


    def process(self):
        pass

    def receive(self):
        request = None
        try:
            request = self.socket.recv(4096).decode('utf-8')
        except TimeoutError:
            pass
        return request


    def send(self, message):
        self.socket.send(bytes(message.encode('utf-8')))
