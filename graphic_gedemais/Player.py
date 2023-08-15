
class   Player():
    def __init__(self, x, y, o, lvl):
        self.x = x
        self.y = y
        self.o = o
        self.lvl = lvl
        self.inventory = []
        self.states_queue = []
        self.state = self.o
        self.step = 0
        self.eggs = {}
