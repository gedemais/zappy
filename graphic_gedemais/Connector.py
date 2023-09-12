import socket
from Egg import Egg
from Player import S
from time import sleep

class   Connector():

    def __init__(self, host, port, tick=60):
        # Socket initialization
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Socket connection
        print(host, port)
        self.socket.connect((socket.gethostbyname('localhost'), port))
        # Timeout setting to avoid recv from receiving all the time
        self.tick = tick
#        self.socket.settimeout(1.0 / timeout)
        self.ready = False

        self.event_functions = {
                #  : placeholder implemented
                ## : graphical procedure implemented
                'pnw' : self.pnw, ##
                'ppo' : self.ppo, ##
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
                'enw' : self.enw, #
                'eht' : self.eht, #
                'edi' : self.edi, #
                'pdi' : self.pdi, ##
                'sgt' : self.sgt,
                'seg' : self.seg
        }

        self.commands_functions = {
                'pnw' : None, ##
                'ppo' : self._ppo_, ##
                'ebo' : None, #
                'pgt' : None, #
                'pin' : None, #
                'bct' : None, #
                'pdr' : None, #
                'pex' : None, #
                'pbc' : None, #
                'pic' : None,
                'pie' : None,
                'plv' : None, #
                'pfk' : None, #
                'enw' : None, #
                'eht' : None, #
                'edi' : None, #
                'pdi' : None, ##
                'sgt' : None,
                'seg' : None
        
                }

        self.commands_queue = []


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

        print(len(response))

        self.socket.settimeout(1.0 / self.tick)

        return response


    def process(self, world):

        response = self.receive()
        if response != None:
            lines = response.split('\n')
            for line in lines:
                tokens = line.split(' ')
                if len(tokens) < 2:
                    continue
                elif tokens[0] == 'bct':
                    print(tokens)
                self.event_functions[tokens[0]](world, tokens)

        for command in self.commands_queue:
            command['ticks'] -= 1
            if command['ticks'] == 0:
                command['id'](*command['params'])

        self.commands_queue = [cmd for cmd in self.commands_queue if cmd['ticks'] > 0]

        to_delete = []
        for t in world.teams:
            for egg in world.teams[t].eggs:
                world.teams[t].eggs[egg].cycles -= 1
                if world.teams[t].eggs[egg].cycles == 0:
                    to_delete.append((t, egg))

        for d in to_delete:
            del world.teams[d[0]].eggs[d[1]]


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
            if pid in team.players.keys():
                return team.players[pid]

        return None


    ########### Server signals ############

    def pnw(self, world, tokens):
        self.ready = True
        if len(tokens) != 7:
            print('invalid format for pnw')
            return -1
        world.lines = [" ".join(tokens)]
        world.line_index = 0
        return world.parse_new_player()


    def _ppo_(self, world, tokens):
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        player.x = int(tokens[2])
        player.y = int(tokens[3])
        player.o = int(tokens[4])

        player.state = S.IDLE
        player.step = 1


    def ppo(self, world, tokens):
        if len(tokens) != 5:
            print('invalid format for ppo')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return

        # Determine player position -> if it moved, set walking state to animate

        if player is None:
            return

        new_x, new_y = int(tokens[2]), int(tokens[3])

        player.next_x = new_x
        player.next_y = new_y
        if player.x < new_x:
            player.states_queue.append(S.WALKING_EAST)
        elif player.x > new_x:
            player.states_queue.append(S.WALKING_WEST)
        elif player.y < new_y:
            player.states_queue.append(S.WALKING_NORTH)
        elif player.y > new_y:
            player.states_queue.append(S.WALKING_SOUTH)

        self.commands_queue.append({'id': self._ppo_,  'params': (world, tokens), 'ticks': 7})



    def _ebo_(self, world, tokens):
        pass

    def ebo(self, world, tokens):
        pass


    def _pin_(self, world, tokens):
        pass

    def pin(self, world, tokens):
        if len(tokens) != 11:
            print('invalid format for pin')
            return -1
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        for i in range(4, 11):
            player.inventory[i - 4] = int(tokens[i])


    def _bct_(self, world, tokens):
        pass

    def bct(self, world, tokens):
        if len(tokens) != 10:
            print('invalid format for bct')
            return -1
        world.lines = [' '.join(tokens)]
        world.line_index = 0
        return world.parse_bct(int(tokens[1]), int(tokens[2]))


    def _pgt_(self, world, tokens):
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        loot = int(tokens[2])
        player.inventory[loot] += 1
        world.map[player.y][player.x][loot] -= 1

    def pgt(self, world, tokens):
        if len(tokens) != 3:
            print('invalid format for pgt')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        player.states_queue.append(S.TAKING)

        self.commands_queue.append({'id': self._pgt_,  'params': (world, tokens), 'ticks': 7})


    def _pdr_(self, world, tokens):
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        loot = int(tokens[2])
        player.inventory[loot] -= 1
        world.map[player.y][player.x][loot] += 1

    def pdr(self, world, tokens):
        if len(tokens) != 3:
            print('invalid format for pgt')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        player.states_queue.append(S.PUTTING)

        self.commands_queue.append({'id': self._pdr_,  'params': (world, tokens), 'ticks': 7})


    def _pex_(self, world, tokens):
        pass

    def pex(self, world, tokens):
        if len(tokens) != 2:
            return -1

        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        player.states_queue.append(S.PUSHING)


    def _pbc_(self, world, tokens):
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        player.state = S.IDLE
        player.step = 1

    def pbc(self, world, tokens):
        if len(tokens) < 3:
            print('invalid format for pbc')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        print('player {} broadcasted |{}|'.format(tokens[1], ' '.join(tokens[1:])))
        player.states_queue.append(S.BROADCASTING)
        self.commands_queue.append({'id': self._pbc_,  'params': (world, tokens), 'ticks': 7})


    def _pic_(self, world, tokens):
        player = self.get_player_by_id(world, tokens[4])
        if player is None:
            return
        player.state = S.IDLE
        player.step = 1

    def pic(self, world, tokens):
        if len(tokens) < 5:
            print('invalid format for pic')
            return -1

        player = self.get_player_by_id(world, tokens[4])
        if player is None:
            return
        print('player {} incantating !'.format(tokens[4]))
        player.states_queue.append(S.INCANTATING)
        self.commands_queue.append({'id': self._pic_,  'params': (world, tokens), 'ticks': 300})


    def _pie_(self, world, tokens):
        pass

    def pie(self, world, tokens):
        pass


    def _plv_(self, world, tokens):
        pass

    def plv(self, world, tokens):
        if len(tokens) != 3:
            print('invalid format for plv')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        player.lvl = int(tokens[2])


    def _pfk_(self, world, tokens):
        player = self.get_player_by_id(world, tokens[1])

        player.state = S.IDLE
        player.step = 1

    def pfk(self, world, tokens):
        if len(tokens) != 2:
            print('invalid format for pfk')
            return -1

        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        print('player {} laying egg !'.format(tokens[1], ' '.join(tokens[1:])))
        player.states_queue.append(S.LAYING_EGG)
        self.commands_queue.append({'id': self._pfk_,  'params': (world, tokens), 'ticks': 42})


    def _enw_(self, world, tokens):
        pass

    def enw(self, world, tokens):
        player = self.get_player_by_id(world, tokens[2])
        if player is None:
            return
        x = int(tokens[3])
        y = int(tokens[4])
        world.teams[player.team].eggs[tokens[1]] = Egg(x, y) # Update every egg at every tick
        print(world.teams[player.team].eggs)
        #sleep(3)


    def _eht_(self, world, tokens):
        pass

    def eht(self, world, tokens):
        print('egg {} hatched !'.format(tokens[1]))
        #sleep(3)
        for t in world.teams.items():
            print(t[1].eggs)
            for egg in t[1].eggs.items():
                print(tokens[1], egg)
                if tokens[1] == egg[0]:
                    del t[1].eggs[tokens[1]]
                    print('properly !')
                    #sleep(3)
                    return 0

        print('UNPROPERLY')
        #sleep(3)

    def _edi_(self, world, tokens):
        pass

    def edi(self, world, tokens):
        print('egg {} rotted !'.format(tokens[1]))
        #sleep(3)
        for t in world.teams.items():
            for egg in t[1].eggs.items():
                print(tokens[1], egg)
                if tokens[1] == egg[0]:
                    del t[1].eggs[tokens[1]]
                    print('properly !')
                    #sleep(3)
                    return 0

        print('UNPROPERLY')
        #sleep(3)

    def _pdi_(self, world, tokens):
        found = False
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        team = world.teams[player.team]
        del team.players[tokens[1]]
        found = True

        if found == False:
            print('dead player {} not found'.format(tokens[1]))
            return -1

        print('player {} killed properly !'.format(tokens[1]))
        return 0

    def pdi(self, world, tokens):
        print('player died !')
        if len(tokens) != 2:
            print('invalid format for pdi')
            return -1
        player = self.get_player_by_id(world, tokens[1])
        if player is None:
            return
        player.states_queue.append(S.DYING)
        self.commands_queue.append({'id': self._pdi_,  'params': (world, tokens), 'ticks': 42})





    def _sgt_(self, world, tokens):
        pass

    def sgt(self, world, tokens):
        if len(tokens) != 2:
            print('invalid format for sgt')
            return -1
        self.tick = int(tokens[1])


    def _seg_(self, world, tokens):
        pass

    def seg(self, world, tokens):
        print('GAME OVER')
        pass
