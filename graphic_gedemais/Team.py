import pygame

class   Team():
    def __init__(self, name):
        self.name = name
        self.players = {}
        self.eggs = {}

    def to_dict(self, renderer, world):

        if len(self.players) == 0:
            return {}
        for player in self.players.items():
            team_index = player[1].team_index
            break

        player_list = [player[1].to_dict() for player in self.players.items()]
        return {'name': self.name, 'players': player_list, 'spritesheet': renderer.spritesheet_paths[team_index]}

