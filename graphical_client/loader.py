import pygame

class PlayerLoader(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()

        self.frame_delay = 2  # Adjust this value to change the animation speed
        self.frame_counter = 0 # work with the delay

        self.frame_index = 0 # index pointing the good image for the animation actually running

        # define if the animation must run or not
        self.animation_finished = False
        self.current_animation = 'walk_down' # the current animation

        # list of differents animations possible
        self.animation_frames = {
            'walk_right': [],
            'walk_down': [],
            'walk_left': [],
            'walk_up': [],

            'collect_up': [],
            'collect_left': [],
            'collect_down': [],
            'collect_right': [],

            'levelup_up': [],
            'levelup_down': [],
            'levelup_left': [],
            'levelup_right': [],

            'push_up': [],
            'push_left': [],
            'push_down': [],
            'push_right': [],

            'standing': [],
        }

    def load_animations(self):
        # Load animation frames from image files and add them to the corresponding lists
        # Example: self.animation_frames['walk'].append(pygame.image.load("path/to/walk/frame.png"))

        # Walking animation
        for i in range(9):  # Replace 9 with the number of max frames by animation

        # walking
            # walking up
            frame = self.image.subsurface(pygame.Rect(i * 64, 8 * 64, 64, 64))
            self.animation_frames['walk_up'].append(frame)

            # walking left
            frame = self.image.subsurface(pygame.Rect(i * 64, 9 * 64, 64, 64))
            self.animation_frames['walk_left'].append(frame)

            # walking down
            frame = self.image.subsurface(pygame.Rect(i * 64, 10 * 64, 64, 64))
            self.animation_frames['walk_down'].append(frame)

            # walking right
            frame = self.image.subsurface(pygame.Rect(i * 64, 11 * 64, 64, 64))
            self.animation_frames['walk_right'].append(frame)

            if i < 8:
            
            # collecting
                #collecting up
                frame = self.image.subsurface(pygame.Rect(i * 64, 4 * 64, 64, 64))
                self.animation_frames['collect_up'].append(frame) 
                #collecting left
                frame = self.image.subsurface(pygame.Rect(i * 64, 5 * 64, 64, 64))
                self.animation_frames['collect_left'].append(frame)
                #collecting down
                frame = self.image.subsurface(pygame.Rect(i * 64, 6 * 64, 64, 64))
                self.animation_frames['collect_down'].append(frame)
                #collecting right
                frame = self.image.subsurface(pygame.Rect(i * 64, 7 * 64, 64, 64))
                self.animation_frames['collect_right'].append(frame)

            if i < 7:
            
            # leveling_up
                # Leveling_up up
                frame = self.image.subsurface(pygame.Rect(i * 64, 0, 64, 64))
                self.animation_frames['levelup_up'].append(frame) 
                # Leveling_up down
                frame = self.image.subsurface(pygame.Rect(i * 64, 2 * 64, 64, 64))
                self.animation_frames['levelup_down'].append(frame) 
                # Leveling_up left
                frame = self.image.subsurface(pygame.Rect(i * 64, 64, 64, 64))
                self.animation_frames['levelup_left'].append(frame) 
                # Leveling_up right
                frame = self.image.subsurface(pygame.Rect(i * 64, 3 * 64, 64, 64))
                self.animation_frames['levelup_right'].append(frame) 

            if i < 6:

            # pushing
                # pushing up
                frame = self.image.subsurface(pygame.Rect(i * 64, 12 * 64, 64, 64))
                self.animation_frames['push_up'].append(frame) 
                # pushing down
                frame = self.image.subsurface(pygame.Rect(i * 64, 14 * 64, 64, 64))
                self.animation_frames['push_down'].append(frame) 
                # pushing left
                frame = self.image.subsurface(pygame.Rect(i * 64, 13 * 64, 64, 64))
                self.animation_frames['push_left'].append(frame) 
                # pushing right
                frame = self.image.subsurface(pygame.Rect(i * 64, 14 * 64, 64, 64))
                self.animation_frames['push_right'].append(frame)
            
            # Falling
                frame = self.image.subsurface(pygame.Rect((5 - i) * 64, 14 * 64, 64, 64))
                self.animation_frames['standing'].append(frame)

        # inverse animation
        for i in range(6):

        # reverse pushing
                # pushing up
                frame = self.image.subsurface(pygame.Rect((5 - i) * 64, 12 * 64, 64, 64))
                self.animation_frames['push_up'].append(frame) 
                # pushing down
                frame = self.image.subsurface(pygame.Rect((5 - i) * 64, 14 * 64, 64, 64))
                self.animation_frames['push_down'].append(frame) 
                # pushing left
                frame = self.image.subsurface(pygame.Rect((5 - i) * 64, 13 * 64, 64, 64))
                self.animation_frames['push_left'].append(frame) 
                # pushing right
                frame = self.image.subsurface(pygame.Rect((5 - i) * 64, 14 * 64, 64, 64))
                self.animation_frames['push_right'].append(frame)

    def animate(self):
        if not self.animation_finished:
            self.frame_counter += 1
            if self.frame_counter >= self.frame_delay:
                self.position.x += self.vx
                self.position.y += self.vy
                # print(self.current_animation + ' ' + str(self.frame_index))
                self.image = self.animation_frames[self.current_animation][self.frame_index]
                self.frame_index += 1
                if self.frame_index >= len(self.animation_frames[self.current_animation]):
                    self.frame_index = 0
                    self.animation_finished = True
                    if self.current_animation == 'standing':
                        self.current_animation = 'walk_down'
                    elif self.current_animation.split('_')[0] != 'walk':
                        self.current_animation = 'walk_' + self.current_animation.split('_')[1]
                self.frame_counter = 0
                