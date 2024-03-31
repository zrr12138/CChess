import pygame
import os
from enum import Enum
from chess_board import *
from context import *
import pygame_gui

# pygame setup
pygame.init()
window_size = (1080, 720)
screen = pygame.display.set_mode(window_size)
pygame.display.set_caption("CChess")
gui_manager = pygame_gui.UIManager(window_size,
                                   starting_language='zh')
clock = pygame.time.Clock()
ctx = Context()

ctx.board.set_chess(1, 1, Chess(ChessType.Pao, True))


def handle_event(ctx: Context, event: pygame.event.Event):
    if event.type == pygame.MOUSEBUTTONDOWN:
        mouse_pos = pygame.mouse.get_pos()
        x, y = ChessBoard.pixi_to_chess(mouse_pos[0], mouse_pos[1])
        print(x, y)
    if event.type == pygame.KEYDOWN:
        if event.key == pygame.K_h:
            text_box.set_text("")
    if event.type == pygame.QUIT:
        ctx.running = False


chess_borad = pygame.image.load(os.path.join("image", "chessboardafter.png"))
think_text_box = pygame_gui.elements.UITextBox(
    "",
    relative_rect=pygame.Rect((chess_borad.get_rect().width, 0),
                              (window_size[0] - chess_borad.get_rect().width, chess_borad.get_rect().height)),
    manager=gui_manager
)
help_text_box = pygame_gui.elements.UITextBox(
    "s-开始搜索 d-结束搜索 i-初始棋盘 r-棋盘翻转 m-立即出子",
    relative_rect=pygame.Rect(0, chess_borad.get_rect().height, chess_borad.get_rect().width, 50),
    manager=gui_manager
)

while ctx.running:
    for event in pygame.event.get():
        print(event)
        gui_manager.process_events(event)
        handle_event(ctx, event)

    gui_manager.update(pygame.time.Clock().tick(60) / 1000.0)
    screen.blit(chess_borad, pygame.Rect(0, 0, chess_borad.get_rect().width, chess_borad.get_rect().height))
    ctx.board.draw(chess_borad)
    think_text_box.set_text(ctx.think_text)
    gui_manager.draw_ui(screen)
    pygame.display.flip()

pygame.quit()
