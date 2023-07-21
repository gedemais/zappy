from constantes import *
from player import *
class Team:
    def __init__(self, name, maxId):
        self.name = name
        self.teamId = maxId + 1
        self.players = []

class Cell:
    def __init__(self, x , y):
        self.position = (x, y)
        self.players = []  # List of player ids present in this cell
        self.resources = [
            0, # 0 'food'
            0, # 1 'linemate'
            0, # 2 'deraumere'
            0, # 3 'sibur'
            0, # 4 'mendiane': 0,
            0, # 5 'phiras': 0,
            0, # 6 'thystame': 0
        ]
        
    def update(self, resources):
        self.resources = resources

class GameBoard:
    def __init__(self, width, height):
        self.timeUnit = 0
        self.width = width
        self.height = height
        # self.hub_space = 280
        self.cells = [[Cell(x, y) for x in range(width)] for y in range(height)]
        self.nbTeams = 0
        self.teams = []
        self.players = []
        self.eggs = []
        
    def handleKeys(self, keys):
        # Handle exit button
        if keys[K_ESCAPE]:
            pygame.event.post(QUIT)
    
    def getPlayer(self, playerId):
        player = list(filter(
            lambda player: player.id == playerId, self.players
        ))[0]
        
        return player
    
    def getEgg(self, eggId):
        egg = list(filter(
            lambda egg: egg.id == eggId, self.eggs
        ))[0]
        
        return egg

    def applyCmd(self, cmdLine):
        # apply a command from the server
        pass

    def send(self, cmd):
        # send a command to the server
        pass

    def render(self, surface):
        cell_width =  (surface.get_width() - 100) / self.width
        cell_height = (surface.get_height() - 100) / self.height
        area = pygame.Surface(((surface.get_width() - 100), (surface.get_height() - 100)))
        area.fill(pygame.color.Color(BLACK))

        for y, row in enumerate(self.cells):
            for x, cell in enumerate(row):
                # Draw each cell as a rectangle on the surface
                rect = pygame.Rect(x * cell_width, y * cell_height, cell_width, cell_height)
                # Draw brown border
                pygame.draw.rect(area, pygame.color.Color(WHITE), rect, 1)

                # Draw resources in cell
                for resource in cell.resources:
                    if resource.quantity > 0:
                        for i in range(1, resource.quantity):
                            # Draw resource sprite here
                            resource_sprite = resource.image
                            # Scale sprite to fit within cell
                            resource_sprite = pygame.transform.scale(resource_sprite, (cell_width, cell_height))
                            surface.blit(resource_sprite, (x * cell_width + i, y * cell_height + i))

                # # Draw players in cell
                # for player in cell.players:
                #     # Draw player sprite here
                #     playerSprite = pygame.image.load(f'{player.race}SpriteSheet.png').convert()
                #     # Scale sprite to fit within cell
                #     playerSprite = pygame.transform.scale(playerSprite, (cell_width, cell_height))
                #     surface.blit(playerSprite, (x * cell_width, y * cell_height))
        
        surface.blit(area, (50, 50))

##################################################################################
# command for the server
##################################################################################

    def initializeGame(self, response):
        # "BIENVENUE\n"
        pass
    
    def connect(self):
        # "BIENVENUE\n"
        # "msz X Y\n"
        # "sgt T\n"
        # "bct 0 0 q q q q q q q\n"
        # …
        # "bct X Y q q q q q q q\n"
        # "tna N\n"
        # …
        # "tna N\n"
        # "pnw #n X Y O L N\n"
        # …
        # "enw #e #n X Y\n"
        # …
        self.send(f'GRAPHIC\n')

    def getMapSize(self):
        # Ask the size of the map
        self.send(f'msz\n')

    def getCell(self, x, y):
        # Ask the content of a cell
        self.send(f'bct {x} {y}\n')

    def getMap(self):
        # Ask content of all the map
        self.send(f'mct\n')

    def getTeams(self):
        # Ask name of the teams
        self.send(f'tna\n')
    
    def getPlayerPosition(self, player):
        # Ask a player position
        self.send(f'ppo {player.playerId}\n')

    def playerLevel(self, player):
        # Ask player level
        self.send(f'piv {player.playerId}\n')

    def playerInventory(self, player):
        # Ask the player's inventory
        self.send(f'pin {player.playerId}\n')

    def getTimeUnit(self):
        # Ask current time unit.
        # response = "sgt T\n"
        self.send(f'sgt\n')

    def modifyTimeUnit(self, timeUnit):
        # Ask to modify time unit.
        # response = "sgt T\n"
        self.send(f'sst {timeUnit}\n')

##################################################################################
# message from server
##################################################################################

    def receiveMapSize(self, x, y):
        # "msz X Y\n"
        # update map info
        self.width = x
        self.height = y
    
    def updateCell(self, x, y, resources):
        # "bct X Y q q q q q q q\n"
        self.cells[y][x].update(resources)

    def addTeam(self, teamName):
        # "tna N\n"
        team = Team(teamName, self.nbTeams)
        self.teams.push(team)
        self.nbTeams += 1

    def newPlayerConnexion(self, playerId, x, y, o, level, teamName):
        # "pnw #n X Y O L N\n"
        newPlayer = Player(self, playerId, teamName, x, y, o, level)
        self.players.append(newPlayer)

    def updatePlayerPosition(self, playerId, x, y, o):
        # "ppo #n X Y O\n"
        player = self.getPlayer(playerId)
        player.updatePos(x, y, o)

    def updatePlayerLevel(self, playerId, level):
        # "plv #n L\"
        player = self.getPlayer(playerId)
        player.updateLvl(level)
        
    def updateInventory(self, playerId, x, y, resources):
        # "pin #n X Y q q q q q q q\n"
        player = self.getPlayer(playerId)
        player.updateInventory(resources)

    def expulse(self, playerId):
        # "pex #n\n"
        kicker = self.getPlayer(playerId)
        kicker.kick()

    def playerBroadcast(self, playerId, message):
        # "pbc #n M\n"
        broadcaster = self.getPlayer(playerId)
        broadcaster.broadcast(message)

    def incantation(self, x, y, level, incantatorId, playerIds):
        # "pic X Y L #n #n …\n"
        incantator = self.getPlayer(incantatorId)
        incantator.incante()
        incantator.levelUping(level)
        players = [self.getPlayer(playerId) for playerId in playerIds]
        for player in players:
            player.levelUping(level)

    def incantationEnd(self, x, y, r):
        # "pie X Y R\n"
        if r:
            self.incantationEndingSuccess(x, y)
        else:
            self.incantationEndingFailure(x, y)

    def layEgg(self, playerId):
        # "pfk #n\n"
        player = self.getPlayer(playerId)
        player.reproduce()

    def throwResource(self, playerId, resourceId):
        # "pdr #n i\n"
        player = self.getPlayer(playerId)
        player.throwResource(resourceId)

    def getResource(self, playerId, resourceId):
        # "pgt #n i\n"
        player = self.getPlayer(playerId)
        player.collectResource(resourceId)

    def playerDeath(self, playerId):
        # "pdi #n\n"
        player = self.getPlayer(playerId)
        player.die()
        self.players.remove(player.getPlayer(playerId))

    def eggLaid(self, eggId, playerId, x, y):
        # "enw #e #n X Y\n"
        self.eggs.append(Egg(eggId, playerId, x, y))
        # add egg apparition animation

    def eggHatch(self, eggId):
        # "eht #e\n"
        egg = self.getEgg(eggId)
        # call hatching animation
        self.eggs.remove(egg)

    def eggConnect(self, eggId):
        # "ebo #e\n"
        egg = self.getEgg(eggId)
        egg.connected = True

    def eggStarvation(self, eggId):
        # "edi #e\n"
        egg = self.getEgg(eggId)
        # call egg rotting animation
        self.eggs.remove(egg)
        
    def updateTimeUnit(self, t):
        # "sgt T\n"
        self.timeUnit = t

    def endGame(self, team_name):
        # "seg N\n"
        # call winning animation with the team name.
        # Quit Pygame
        pygame.quit()
        raise SystemExit        

    def serverMsg(self, message):
        # "smg M\n"
        pass

    def unknownCmd(self):
        # "suc\n"
        pass

    def badParameters():
        # "sbp\n"
        pass
