
class   World():

    def __init__(self, response):
        self.players = []
        self.parse_graphical_data(response)


    def parse_graphical_data(self, response):
        self.lines = response.split('\n')
        self.line_index = 0

        if self.parse_map_size() != 0:
            exit(1)
        self.map = [[[] for x in range(self.map_width)] for y in range(self.map_height)]
        self.line_index += 1

        if self.parse_t() != 0:
            exit(1)
        self.line_index += 1

        for y in range(self.map_height):
            for x in range(self.map_width):
                if self.parse_bct(x, y) != 0:
                    exit(1)
                print(self.map[y][x])
                self.line_index += 1



    def parse_map_size(self):
        tokens = self.lines[self.line_index].split(' ')
        if len(tokens) != 3 or tokens[0] != 'msz':
            return -1

        try:
            self.map_width = int(tokens[1])
            self.map_height = int(tokens[2])
            print(self.map_width, self.map_height)
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
            return -1

        try:
            if int(tokens[1]) != x or int(tokens[2]) != y:
                return -1

            for i in range(3, 10):
                self.map[y][x].append(int(tokens[i]))
        except:
            return -1

        return 0

    #def parse_new_player(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
    #def parse_(self):
        #tokens = self.lines[self.line_index].split(' ')
