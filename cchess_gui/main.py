import pygame
import os
from enum import Enum
from chess_board import *

# pygame setup
pygame.init()
screen = pygame.display.set_mode((1280, 720))
clock = pygame.time.Clock()
running = True

chess_borad = pygame.image.load(os.path.join("image", "chessboardafter.png"))
borad = ChessBoard()
borad.board[1][1] = Chess(ChessType.Ju, True)
chess_group = borad.to_sprite_group()
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
    screen.blit(chess_borad, pygame.Rect(0, 0, chess_borad.get_rect().width, chess_borad.get_rect().height))

    chess_group.draw(chess_borad)
    # flip() the display to put your work on screen
    pygame.display.flip()

    clock.tick(60)  # limits FPS to 60

pygame.quit()
