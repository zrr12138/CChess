import pygame
import os
from enum import Enum
# pygame setup
pygame.init()
screen = pygame.display.set_mode((1280, 720))
clock = pygame.time.Clock()
running = True
class Color(Enum):
    RED=0
    BLACK=1

class Type(Enum):
    Wang=0
    Ma=1
    Bing=2
    Shi=3
    Ju=4
    Pao=5
    Xiang=6

def Color2Str(color:Color)->str:
    colors=["red","black"]
    return colors[color.value]

def Type2Str(type:Type)->str:
    types=["wang","ma","bing","shi","ju","pao","xiang"]
    return types[type.value]

class Chess(pygame.sprite.Sprite):

    # Constructor. Pass in the color of the block,
    # and its x and y position
    def __init__(self, color:Color, type:Type, row, col):
       # Call the parent class (Sprite) constructor
       pygame.sprite.Sprite.__init__(self)
       
       self.image = pygame.image.load(os.path.join("image",Color2Str(color)+'-'+Type2Str(type)+".png"))
       bias=27
       begin_x=70
       begin_y=70
       self.rect = self.image.get_rect()
       self.rect.x=begin_x+54*col-bias
       self.rect.y=begin_y+54*row-bias

       mask = pygame.mask.from_surface(self.image)
       outline = mask.outline()
       pygame.draw.polygon(self.image, (217, 175, 111, 0), outline, 7)

       


chess_borad=pygame.image.load(os.path.join("image","chessboardafter.png"))
b_bing1=Chess(Color.BLACK,Type.Bing,0,0)
b_bing2=Chess(Color.BLACK,Type.Bing,0,1)
chess_group=pygame.sprite.Group()
chess_group.add(b_bing1)
chess_group.add(b_bing2)

while running:
    # poll for events
    # pygame.QUIT event means the user clicked X to close your window
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            mouse_pos = pygame.mouse.get_pos()
            for chess in chess_group:
                if chess.rect.collidepoint(mouse_pos):
                    # 点击了chess
                    break
            



    # fill the screen with a color to wipe away anything from last frame
    screen.blit(chess_borad,pygame.Rect(0,0,chess_borad.get_rect().width,chess_borad.get_rect().height))
    
    chess_group.draw(chess_borad, )
    # flip() the display to put your work on screen
    pygame.display.flip()

    clock.tick(60)  # limits FPS to 60

pygame.quit()