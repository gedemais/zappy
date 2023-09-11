from time import sleep
from Team import Team
from Player import Player
from Egg import Egg

class   World():

    def __init__(self, response):
        self.teams = {}
        self.team_names = []
        self.team_index = 0
        self.parse_graphical_data(response)

    def get_player_by_id(self, pid):
        for team in self.teams.items():
            team = team[1]
            if pid in team.players.keys():
                return team.players[pid]

        return None

    def parse_graphical_data(self, response):
        self.lines = response.split('\n')
        self.line_index = 0

        if self.parse_map_size() != 0:
            print('map size parsing failed !')
            exit(1)
        self.map = [[[] for x in range(self.map_width)] for y in range(self.map_height)]
        self.line_index += 1

        if self.parse_t() != 0:
            print('t parsing failed !')
            exit(1)
        self.line_index += 1

        for y in range(self.map_height):
            for x in range(self.map_width):
                if self.parse_bct(x, y) != 0:
                    print(x, y)
                    print('block content parsing failed !')
                    exit(1)
                self.line_index += 1

        while self.parse_team_name() == 0:
            self.line_index += 1

        while self.parse_new_player() == 0:
            print(self.lines[self.line_index])
            self.line_index += 1

        while self.parse_new_egg() == 0:
            print(self.lines[self.line_index])
            self.line_index += 1



############ Parsing utils

    def parse_map_size(self):
        tokens = self.lines[self.line_index].split(' ')
        if len(tokens) != 3 or tokens[0] != 'msz':
            return -1

        try:
            self.map_width = int(tokens[1])
            self.map_height = int(tokens[2])
        except:
            return -1
        return 0


    def parse_t(self):
        tokens = self.lines[self.line_index].split(' ')

        if len(tokens) != 2 or tokens[0] != 'sgt':
            return -1
        try:
            self.t = int(tokens[1])
        except:
            return -1
        return 0


    def parse_bct(self, x, y):
        tokens = self.lines[self.line_index].split(' ')
        if len(tokens) != 10 or tokens[0] != 'bct':
            print('invalid format')
            return -1

        try:
            if int(tokens[1]) != x or int(tokens[2]) != y:
                print('invalid position')
                return -1

            for i in range(3, 10):
                self.map[y][x].append(int(tokens[i]))
        except:
            return -1

        return 0


    def parse_team_name(self):
        tokens = self.lines[self.line_index].split(' ')
        if len(tokens) != 2 or tokens[0] != 'tna':
            print('invalid format')
            return -1

        self.teams[tokens[1]] = Team(tokens[1])
        return 0


    def parse_new_player(self):
        tokens = self.lines[self.line_index].split(' ')
        if len(tokens) != 7 or tokens[0] != 'pnw':
            print('invalid format for pnw')
            return -1

        x = int(tokens[2])
        y = int(tokens[3])
        o = int(tokens[4])
        l = int(tokens[5])
        team = tokens[6]

        if team not in self.team_names:
            self.team_names.append(team)

        team_index = self.team_names.index(team)
        # Protect against overflow
        self.teams[team].players[tokens[1]] = Player(x, y, o, l, team, team_index)
        return 0

    def parse_new_egg(self):
        tokens = self.lines[self.line_index].split(' ')
        print(tokens)
        if len(tokens) != 5 or tokens[0] != 'enw':
            print('invalid format for enw')
            return -1

        player = self.get_player_by_id(tokens[2])
        x = int(tokens[3])
        y = int(tokens[4])
        self.teams[player.team].eggs[tokens[1]] = Egg(x, y) # Update every egg at every tick

        return 0
