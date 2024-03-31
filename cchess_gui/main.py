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
        # 鼠标右键
        if event.button == 3 and ctx.status == Status.PLACE:
            print("here")
            ctx.board.clear_chess(x, y)
        if event.button == 1:
            if ctx.choose_pos is None:
                ctx.choose_pos = (x, y)
            else:
                if ctx.status == Status.PLACE:
                    # TODO(zrr) chess 默认是引用赋值
                    chess=ctx.board.get_chess(ctx.choose_pos[0], ctx.choose_pos[1])
                    print(chess)
                    ctx.board.set_chess(x, y, chess)
                    ctx.board.clear_chess(ctx.choose_pos[0], ctx.choose_pos[1])
                    ctx.choose_pos = None
                else:
                    move = Move(ctx.choose_pos[0], ctx.choose_pos[1], x, y)
                    pass
                    # 如果不可移动，应该选中新的点

    if event.type == pygame.KEYDOWN:
        if ctx.status == Status.SEARCHING:
            if event.key == pygame.K_d:
                pass
            elif event.key == pygame.K_m:
                pass
        elif ctx.status == Status.PAUSE:
            if event.key == pygame.K_s:
                pass
            elif event.key == pygame.K_r:
                pass
            elif event.key == pygame.K_p:
                ctx.status = Status.PLACE
            elif event.key == pygame.K_i:
                pass
        elif ctx.status == Status.PLACE:
            chess_type = None
            if event.key == pygame.K_w:
                chess_type = ChessType.Wang
            elif event.key == pygame.K_m:
                chess_type = ChessType.Ma
            elif event.key == pygame.K_b:
                chess_type = ChessType.Bing
            elif event.key == pygame.K_s:
                chess_type = ChessType.Shi
            elif event.key == pygame.K_j:
                chess_type = ChessType.Ju
            elif event.key == pygame.K_p:
                chess_type = ChessType.Pao
            elif event.key == pygame.K_x:
                chess_type = ChessType.Xiang
            if chess_type is not None:
                is_red = ctx.place_red
                mouse_pos = pygame.mouse.get_pos()
                x, y = ChessBoard.pixi_to_chess(mouse_pos[0], mouse_pos[1])
                ctx.board.set_chess(x, y, Chess(chess_type, is_red))
            if event.key == pygame.K_TAB:
                ctx.place_red = not ctx.place_red
            if event.key == pygame.K_d:
                ctx.status = Status.PAUSE

        #
        # elif ctx.status == Status.PAUSE:
        #
        # elif ctx.status == Status.PLACE:

    if event.type == pygame.QUIT:
        ctx.running = False


chess_board = ctx.board.image
think_text_box = pygame_gui.elements.UITextBox(
    "",
    relative_rect=pygame.Rect((chess_board.get_rect().width, 0),
                              (window_size[0] - chess_board.get_rect().width, chess_board.get_rect().height)),
    manager=gui_manager
)
help_text_box = pygame_gui.elements.UITextBox(
    "",
    relative_rect=pygame.Rect(0, chess_board.get_rect().height, chess_board.get_rect().width,
                              window_size[1] - chess_board.get_rect().height),
    manager=gui_manager
)

while ctx.running:
    for event in pygame.event.get():
        print(event)
        gui_manager.process_events(event)
        handle_event(ctx, event)
    screen.fill((255, 255, 255))
    gui_manager.update(pygame.time.Clock().tick(60) / 1000.0)
    ctx.board.draw(screen)
    think_text_box.set_text(ctx.think_text)
    help_text_box.set_text(ctx.help_text)
    gui_manager.draw_ui(screen)
    pygame.display.flip()
    pygame.display.update()

pygame.quit()
