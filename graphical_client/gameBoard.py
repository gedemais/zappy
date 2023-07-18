import random

from constantes import *

class Team:
    def __init__(self, name, maxId):
        self.name = name
        self.teamId = maxId
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
        self.cells = [[Cell(x, y) for x in range(width)] for y in range(height)]
        self.teams = []
        self.player = []

    def initializeGame(self, response):
        pass

    # Only for debug purpose
    def spawn_resources(self):
        for row in self.cells:
            for cell in row:
                 # Increase the chance of spawning resources
                if random.random() < 0.3:
                    cell.resources[random.choice(list(cell.resources.keys()))] += random.randint(1, 3)  # Add 1 to 3 resources

    def applyCmd(self, cmdLine):
        pass

    def send(self, cmd):
        # send a command to the server
        pass

    def connect(self):
        # Connection process to server
        response = self.send(f'GRAPHIC\n')
        self.initializeGame(response)
        pass

# command for the server
    def updateMapSize(self):
        # Taille de la carte.
        # response = ""
        response = self.send(f'msz\n')
        # update map

    def watchCell(self, x, y):
        # Contenu d’une case de la carte
        # response = ""
        response = self.send(f'bct {x} {y}\n')
        # update map

    def updateMap(self):
        # Contenu de la carte (toutes les cases).
        # response = ""
        response = self.send(f'mct\n')
        # show the content of the cell

    def updateTeam(self):
        # Nom des équipes.
        # response = ""
        response = self.send(f'tna\n')
        # update teams
    
    def playerPosition(self, player):
        # Position d’un joueur.
        # response = ""
        response = self.send(f'ppo {player.player_id}\n')
        # update visu

    def PlayerLevel(self, player):
        # Niveau d’un joueur.
        # response = ""
        response = self.send(f'piv {player.player_id}\n')
        #update visu

    def PlayerInventory(self, player):
        # Inventaire d’un joueur.
        # response = ""
        response = self.send(f'pin {player.player_id}\n')
        # update visu

    def UpdateTimeUnit(self):
        # Demande de l’unité de temps courante sur le serveur.
        # response = "sgt T\n"
        response = self.send(f'sgt\n')
        # update visu

    def ModifyTimeUnit(self, timeUnit):
        # Modification de l’unité de temps sur le serveur.
        # response = "sgt T\n"
        response = self.send(f'sst {timeUnit}\n')
        # update visu

# message from server
    def newPlayerConnexion(self, playerID, x, y, o, level, teamName):
        # Connexion d’un nouveau joueur.
        # message = "pnw #n X Y O L N\n"
        # add player to list and update teams
        pass

    def expulse(self, player_id):
        # Un joueur expulse.
        # message = "pex #n\n"
        # call move method on all player on the same case in function of the orientation of the player_id player
        pass

    def playerBroadcast(self, player_id, message):
        # Un joueur fait un broadcast.
        # message = "pbc #n M\n"
        pass

    def launchIncantation(self, x, y, launcher_id, player_ids):
        # Premier joueur lance l’incantation pour tous les suivants sur la case.
        # message = "pic X Y L #n #n …\n"
        pass

    def EndIncantation(self, x, y, result):
        # Fin de l’incantation sur la case donnée avec le résultat R (0 ou 1).
        # message = "pie X Y R\n"
        pass

    def layEgg(self, player):
        # Le joueur pond un œuf.
        # message = "pfk #n\n"
        pass

    def throwResource(self, player, resource_id):
        # Le joueur jette une ressource.
        # message = "pdr #n i\n"
        pass

    def getResource(self, player, resource_id):
        # Le joueur prend une ressource.
        # message = "pgt #n i\n" 
        pass

    def playerDeath(self, player):
        # Le joueur est mort de faim.
        # message = "pdi #n\n"
        pass

    def eggLaid(self, egg_id, player_id, x, y):
        # L’œuf a été pondu sur la case par le joueur.
        # message = "enw #e #n X Y\n"
        pass

    def eggHatch(self, egg_id):
        # L’œuf éclot.
        # message = "eht #e\n"
        pass

    def eggConnect(self, egg_id):
        # Un joueur s’est connecté pour l’œuf.
        # message = "ebo #e\n"
        pass

    def eggStarvation(self, egg_id):
        # L’œuf éclos est mort de faim.
        # message = "edi #e\n"
        pass

    def endGame(self, team_name):
        # Fin du jeu. L’équipe donnée remporte la partie.
        # message = "seg N\n"
        pass

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
