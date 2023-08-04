import socket

class   Connector():

    def __init__(self, host, port, timeout=60):
        # Socket initialization
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Socket connection
        self.socket.connect((host, port))
        # Timeout setting to avoid recv from receiving all the time
        #self.socket.settimeout(1.0 / timeout)

        self.event_functions = {
                'pnw' : self.pnw,
                'ppo' : self.ppo,
                'ebo' : self.ebo,
                'pgt' : self.pgt,
                'pin' : self.pin,
                'bct' : self.bct,
                'pdr' : self.pdr,
                'pex' : self.pex,
                'pbc' : self.pbc,
                'pic' : self.pic,
                'pie' : self.pie,
                'plv' : self.plv,
                'pfk' : self.pfk,
                'enw' : self.enw,
                'eht' : self.eht,
                'edi' : self.edi,
                'pdi' : self.pdi,
                'sgt' : self.sgt,
                'sst' : self.sst,
                'seg' : self.seg
        }


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


    def process(self, world):

        response = self.receive()
        if response == None:
            return

        lines = response.split('\n')
        for line in lines:
            print('----------')
            print(line)
            print('----------')
            tokens = line.split(' ')
            self.event_functions[tokens[0]](world, tokens)
            

    def receive(self):
        request = None
        try:
            request = self.socket.recv(65536).decode('utf-8')
        except TimeoutError:
            pass
        return request


    def send(self, message):
        self.socket.send(bytes(message.encode('utf-8')))

    ########### Server signals ############

    def pnw(self, world, tokens):
        pass


    def ppo(self, world, tokens):
        for team in world.teams.items():
            if tokens[1] in team.players.keys():
                print('ppo successful')
                team.players[tokens[1]].x = int(tokens[2])
                team.players[tokens[1]].y = int(tokens[3])
                team.players[tokens[1]].o = int(tokens[4])


    def ebo(self, world, tokens):
        pass


    def pgt(self, world, tokens):
        pass


    def pin(self, world, tokens):
        pass


    def bct(self, world, tokens):
        pass


    def pdr(self, world, tokens):
        pass


    def pex(self, world, tokens):
        pass


    def pbc(self, world, tokens):
        pass


    def pic(self, world, tokens):
        pass


    def pie(self, world, tokens):
        pass


    def plv(self, world, tokens):
        pass


    def pfk(self, world, tokens):
        pass


    def enw(self, world, tokens):
        pass


    def eht(self, world, tokens):
        pass


    def edi(self, world, tokens):
        pass


    def pdi(self, world, tokens):
        pass


    def sgt(self, world, tokens):
        pass


    def sst(self, world, tokens):
        pass


    def seg(self, world, tokens):
        pass
