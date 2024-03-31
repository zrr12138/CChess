import pygame
import os
from enum import Enum
from chess_board import *
from context import *

# pygame setup
pygame.init()
screen = pygame.display.set_mode((1280, 720))
clock = pygame.time.Clock()
ctx = Context()

ctx.board.set_chess(1, 1, Chess(ChessType.Pao, True))


def handle_event(ctx: Context, event: pygame.event.Event):
    if event.type == pygame.MOUSEBUTTONDOWN:
        mouse_pos = pygame.mouse.get_pos()
        x, y = ChessBoard.pixi_to_chess(mouse_pos[0], mouse_pos[1])
        print(x, y)
    if event.type == pygame.QUIT:
        ctx.running = False

chess_borad = pygame.image.load(os.path.join("image", "chessboardafter.png"))

while ctx.running:
    for event in pygame.event.get():
        print(event)
        handle_event(ctx, event)

    screen.blit(chess_borad, pygame.Rect(0, 0, chess_borad.get_rect().width, chess_borad.get_rect().height))
    ctx.board.draw(chess_borad)
    pygame.display.flip()
    clock.tick(60)  # limits FPS to 60

pygame.quit()
