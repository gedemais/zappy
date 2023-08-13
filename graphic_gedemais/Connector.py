import socket

class   Connector():

    def __init__(self, host, port, timeout=60):
        # Socket initialization
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Socket connection
        print(host, port)
        self.socket.connect((socket.gethostbyname('localhost'), port))
        # Timeout setting to avoid recv from receiving all the time
        self.timeout = timeout
#        self.socket.settimeout(1.0 / timeout)

        self.event_functions = {
                'pnw' : self.pnw, #
                'ppo' : self.ppo, #
                'ebo' : self.ebo, #
                'pgt' : self.pgt, #
                'pin' : self.pin, #
                'bct' : self.bct, #
                'pdr' : self.pdr, #
                'pex' : self.pex, #
                'pbc' : self.pbc, #
                'pic' : self.pic,
                'pie' : self.pie,
                'plv' : self.plv, #
                'pfk' : self.pfk, #
                'enw' : self.enw,
                'eht' : self.eht,
                'edi' : self.edi,
                'pdi' : self.pdi, #
                'sgt' : self.sgt,
                'sst' : self.sst,
                'seg' : self.seg
        }


    def authenticate(self):
        self.send('GRAPHICAL\n')

        self.socket.settimeout(10.0)
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

        self.socket.settimeout(1.0 / self.timeout)

        return response


    def process(self, world):

        response = self.receive()
        if response == None:
            return

        lines = response.split('\n')
        for line in lines:
            tokens = line.split(' ')
            if len(tokens) < 2:
                break
            print(tokens)
            self.event_functions[tokens[0]](world, tokens)


    def receive(self):
        request = None
        try:
            request = self.socket.recv(65536).decode('utf-8')
        except socket.timeout:
            pass
        return request


    def send(self, message):
        self.socket.send(bytes(message.encode('utf-8')))


    def get_player_by_id(self, world, pid):
        for team in world.teams.items():
            team = team[1]
            if tokens[1] in team.players.keys():
                return team.players[tokens[1]]

        return None


    ########### Server signals ############

    def pnw(self, world, tokens):
        if len(tokens) != 7:
            print('invalid format for pnw')
            return -1
        world.lines = [" ".join(tokens)]
        world.line_index = 0
        return world.parse_new_player()


    def ppo(self, world, tokens):

        if len(tokens) != 5:
            print('invalid format for ppo')
            return -1

            player = self.get_player_by_id(world, tokens[1])
            player.x = int(tokens[2])
            player.y = int(tokens[3])
            player.o = int(tokens[4])


    def ebo(self, world, tokens):
        print('egg EX-CLOSIOOOOOOOONNN!!!!')
        pass


    def pin(self, world, tokens):

        if len(tokens) != 11:
            print('invalid format for pin')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        for i in range(4, 11):
            player.inventory[i] = int(tokens[i])


    def bct(self, world, tokens):
        if len(tokens) != 10:
            print('invalid format for bct')
            return -1

        world.lines = [' '.join(tokens)]
        world.line_index = 0
        return world.parse_bct(int(tokens[1]), int(tokens[2]))


    def pgt(self, world, tokens):
        if len(tokens) != 3:
            print('invalid format for pgt')
            return -1

        player = get_player_by_id(world, tokens[1])
        loot = int(tokens[2])
        player.inventory[loot] += 1
        world.map[player.y][player.x][loot] -= 1


    def pdr(self, world, tokens):
        if len(tokens) != 3:
            print('invalid format for pgt')
            return -1

        player = get_player_by_id(world, tokens[1])
        loot = int(tokens[2])
        player.inventory[loot] -= 1
        world.map[player.y][player.x][loot] += 1


    def pex(self, world, tokens):
        if len(tokens) != 2:
            return -1

        player = self.get_player_by_id(world, tokens[1])
        print('{} kicked !'.format(tokens[0]))


    def pbc(self, world, tokens):
        if len(tokens) < 3:
            print('invalid format for pbc')
            return -1

        player = get_player_by_id(world, tokens[1])
        print('player {} broadcasted |{}|'.format(tokens[1], ' '.join(tokens[2])))


    def pic(self, world, tokens):
        pass


    def pie(self, world, tokens):
        pass


    def plv(self, world, tokens):
        if len(tokens) != 3:
        pass


    def pfk(self, world, tokens):
        player = get_player_by_id(world, tokens[1])

        


    def enw(self, world, tokens):
        pass


    def eht(self, world, tokens):
        pass


    def edi(self, world, tokens):
        pass


    def pdi(self, world, tokens):
        found = False
        print('player died !')
        if len(tokens) != 2:
            print('invalid format for pdi')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        del team.players[tokens[1]]
        found = True

        if found == False:
            print('dead player {} not found'.format(tokens[1]))
            return -1
        return 0


    def sgt(self, world, tokens):
        pass


    def sst(self, world, tokens):
        pass


    def seg(self, world, tokens):
        pass
