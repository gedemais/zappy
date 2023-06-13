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
        self.resources = {
            'nourriture': 0,
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0
        }

class GameBoard:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        # self.hub_space = 280
        self.cells = [[Cell(x, y) for x in range(width)] for y in range(height)]
        self.teams = []
        self.players = []
        self.eggs = []
        self.spawn_resources()

    def initializeGame(self, response):
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

##################################################################################
# command for the server
##################################################################################

    def getMapSize(self):
        # Taille de la carte.
        response = self.send(f'msz\n')
        # update info

    def getCell(self, x, y):
        # Contenu d’une case de la carte
        response = self.send(f'bct {x} {y}\n')
        # update info if needed

    def getMap(self):
        # Contenu de la carte (toutes les cases).
        response = self.send(f'mct\n')
        # update info if needed

    def getTeams(self):
        # Nom des équipes.
        response = self.send(f'tna\n')
        # update teams
    
    def getPlayerPosition(self, player):
        # Position d’un joueur.
        response = self.send(f'ppo {player.playerId}\n')
        # update player or not

    def playerLevel(self, player):
        # Niveau d’un joueur.
        response = self.send(f'piv {player.playerId}\n')
        #update visu if needed

    def playerInventory(self, player):
        # Inventaire d’un joueur.
        response = self.send(f'pin {player.playerId}\n')
        # update visu if needed

    def getTimeUnit(self):
        # Demande de l’unité de temps courante sur le serveur.
        # response = "sgt T\n"
        response = self.send(f'sgt\n')
        # update visu and animation time

    def updateTimeUnit(self, timeUnit):
        # Modification de l’unité de temps sur le serveur.
        # response = "sgt T\n"
        response = self.send(f'sst {timeUnit}\n')
        # update visu and animation time

##################################################################################
# message from server
##################################################################################

    def newPlayerConnexion(self, playerId, x, y, o, level, teamName):
        # Connexion d’un nouveau joueur.
        # message = "pnw #n X Y O L N\n"
        # add newPlayer to players
        newPlayer = Player(self, playerId, teamName, x, y, o, level)
        self.players.append(newPlayer)

    def expulse(self, playerId):
        # Un joueur expulse.
        # message = "pex #n\n"
        # call move method on all player on the same case in function of the orientation of the playerId player
        kicker = self.getPlayer(playerId)
        # call kick animation on kicker    
        pass

    def playerBroadcast(self, playerId, message):
        # Un joueur fait un broadcast.
        # message = "pbc #n M\n"
        broadcaster = self.getPlayer(playerId)
        # call broadcast animation with the message
        pass

    def launchIncantation(self, x, y, launcherId, playerIds):
        # Premier joueur lance l’incantation pour tous les suivants sur la case.
        # message = "pic X Y L #n #n …\n"
        pass

    def EndIncantation(self, x, y, result):
        # Fin de l’incantation sur la case donnée avec le résultat R (0 ou 1).
        # message = "pie X Y R\n"
        pass

    def layEgg(self, playerId):
        # Le joueur pond un œuf.
        # message = "pfk #n\n"
        player = self.getPlayer(playerId)
        # call animation to lay an egg
        pass

    def throwResource(self, playerId, resourceId):
        # Le joueur jette une ressource.
        # message = "pdr #n i\n"
        player = self.getPlayer(playerId)
        player.throwResource(resourceId)

    def getResource(self, playerId, resourceId):
        # Le joueur prend une ressource.
        # message = "pgt #n i\n"
        player = self.getPlayer(playerId)
        player.collectResource(resourceId)

    def playerDeath(self, playerId):
        # Le joueur est mort de faim.
        # message = "pdi #n\n"
        player = self.getPlayer(playerId)
        player.die()

    def eggLaid(self, eggId, playerId, x, y):
        # L’œuf a été pondu sur la case par le joueur.
        # message = "enw #e #n X Y\n"
        player = self.getPlayer(playerId)
        self.eggs.append(Egg(eggId, playerId, x, y))
        # add egg to sprite


    def eggHatch(self, eggId):
        # L’œuf éclot.
        # message = "eht #e\n"
        egg = self.getEgg(eggId)
        # call hatching animation
        self.eggs.remove(egg)

    def eggConnect(self, eggId):
        # Un joueur s’est connecté pour l’œuf.
        # message = "ebo #e\n"
        egg = self.getEgg(eggId)
        egg.connected = True

    def eggStarvation(self, eggId):
        # L’œuf éclos est mort de faim.
        # message = "edi #e\n"
        egg = self.getEgg(eggId)
        # call egg rotting animation
        self.eggs.remove(egg)
        pass

    def endGame(self, team_name):
        # Fin du jeu. L’équipe donnée remporte la partie.
        # message = "seg N\n"
        # call winning animation with the team name.
        # Quit Pygame
        pygame.quit()
        raise SystemExit        

    def serverMsg(self, message):
        # Message du serveur.
        # message = "smg M\n"
        pass

    def unknownCmd(self):
        # Commande inconnue.
        # message = "suc\n"
        pass

    def badParameters():
        # Mauvais paramètres pour la commande.
        # message = "sbp\n"
        pass

##################################################################################
# events
##################################################################################

    def connect(self):
        # Connexion du moniteur
        # message :
        #
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
        response = self.send(f'GRAPHIC\n')
        self.initializeGame(response)
        pass

    def playerConnection(self):
        # Connexion d’un joueur
        # message = "pnw #n X Y O L N\n"
        
        pass

    def eggPlayerConnection(self):
        # Connexion d’un joueur grâce a un œuf
        # message = "ebo #e\n"
        #           "pnw #n X Y O L N\n"
        pass

    def walk(self, playerId, x, y, o):
        # Avance
        # message = "ppo #n X Y O\n"
        player = self.getPlayer(playerId)
        player.walk() 
        pass

    def right(self, playerId, x, y, o):
        # Droite
        # message = "ppo #n X Y O\n"
        player = self.getPlayer(playerId)
        player.turnRight() 
        pass

    def left(self, playerId, x, y, o):
        # Gauche
        # message = "ppo #n X Y O\n"
        player = self.getPlayer(playerId)
        player.turnLeft() 
        pass

    def watch(self, playerId):
        # Voir
        player = self.getPlayer(playerId)
        player.watch()

    def inventory(self, playerId):
        # Inventaire
        player = self.getPlayer(playerId)
        player.seeInventory()

    def take(self, playerId, resourceId):
        # Prend
        # message :
        # "pgt #n i\n"
        player = self.getPlayer(playerId)
        player.collectResources(resourceId)
        # "pin #n X Y q q q q q q q\n"
        player.seeInventory()
        # "bct X Y q q q q q q q\n"
        # self.updateMap ? not sure it is needed

    def put(self, playerId, resourceId):
        # Pose
        # message :
        # "pdr #n i\n"
        player = self.getPlayer(playerId)
        player.throwResource(resourceId)
        # "pin #n X Y q q q q q q q\n"
        player.seeInventory()
        # "bct X Y q q q q q q q\n"
        # self.updateMap ? not sure it is needed, the map will update when the player throw it.

    def expulse(self):
        # Expulse
        # message :
        # "pex #n\n"
        # "ppo #n X Y O\n"
        # …
        # "ppo #n X Y O\n"
        pass

    def broadcast(self):
        # Broadcast
        # message = "pbc #n M\n"
        pass

    def incantation(self):
        # Incantation
        # message = "pic X Y L #n #n …\n"
        pass

    def incantationEnd(self, ):
        # Fin de l'incantation
        # message :
        # "pie X Y R\n"
        # "plv #n L\n"
        # …
        # "plv #n L\n"
        # "bct X Y q q q q q q q\n"
        # …
        # "bct X Y q q q q q q q\n"
        pass

    def fork(self, playerId):
        # Fork
        # message = "pfk #n\n"
        pass

    def forkEnd(self, eggId, playerId, x, y):
        # Fin du fork
        # message = "enw #e #n X Y\n"
        pass

    def eggHatched(self, eggId):
        # Eclosion de l'oeuf
        # message = "eht #e\n"
        pass

    def eggRotted(self, eggId):
        # Oeuf moisi (mort)
        # message = "edi #e\n"
        pass

    def death(self, playerId):
        # Mort d'un joueur
        # message = "pdi #n\n"
        pass

    def end(self, winningTeam):
        # Fin du jeu
        # message = "seg N\n"
        pass
