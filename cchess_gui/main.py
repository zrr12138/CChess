import pygame
import os
from enum import Enum
from chess_board import *
from context import *
import pygame_gui
from client import Client
import time

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

client = Client("127.0.0.1", 12138)


def handle_chess_board_mouse_button_down(ctx, event, x: int, y: int):
    if event.button == 3 and ctx.status == Status.PLACE:
        ctx.board.clear_chess(x, y)
    if event.button == 1:
        if ctx.choose_pos is None:
            ctx.choose_pos = (x, y)
        else:
            end_chess = ctx.board.get_chess(x, y)
            start_chess = ctx.board.get_chess(ctx.choose_pos[0], ctx.choose_pos[1])
            if ctx.status == Status.PLACE:
                if start_chess.type != ChessType.Empty:
                    ctx.board.set_chess(x, y, start_chess)
                    ctx.board.clear_chess(ctx.choose_pos[0], ctx.choose_pos[1])
                    ctx.choose_pos = None
                else:
                    ctx.choose_pos = (x, y)
            else:
                ctx.board = client.board_move(ctx.board, Move(ctx.choose_pos[0], ctx.choose_pos[1], x, y))
                end = client.board_end(ctx.board)
                if end != BoardResult.NOT_END:
                    ctx.status = Status.FINISH
                ctx.choose_pos = None


def handle_key_down(ctx, event):
    if ctx.status == Status.SEARCHING:
        if event.key == pygame.K_d:
            client.engine_stop()
            ctx.status = Status.PAUSE
        elif event.key == pygame.K_m:
            ctx.board = client.engine_action(ctx.board)
            end = client.board_end(ctx.board)
            if end != BoardResult.NOT_END:
                ctx.status = Status.FINISH

    elif ctx.status == Status.PAUSE:
        if event.key == pygame.K_s:
            if pygame.key.get_mods() & pygame.KMOD_CTRL:
                client.start_search(ctx.board, False)
            else:
                client.start_search(ctx.board, True)
            ctx.status = Status.SEARCHING
            ctx.think_text = ""
        elif event.key == pygame.K_r:
            ctx.board = client.reverse_board(ctx.board)
        elif event.key == pygame.K_p:
            ctx.status = Status.PLACE
        elif event.key == pygame.K_i:
            ctx.board = client.get_init_board()
        elif event.key == pygame.K_l:
            file_dialog = pygame_gui.windows.UIFileDialog(
                pygame.Rect(200, 200, 400, 200),
                gui_manager,
                window_title="选择要打开的 .qp 文件",
                initial_file_path="/home/zrr/cchess_test",
                allow_existing_files_only=True,
            )
        elif event.key == pygame.K_RIGHT:
            print(ctx.record_index)
            if ctx.record and ctx.record_index + 1 < ctx.record.get_total_boards():
                ctx.record_index += 1
                ctx.board = ctx.record.get_board(ctx.record_index)
        elif event.key == pygame.K_LEFT:
            if ctx.record and ctx.record_index - 1 >= 0:
                ctx.record_index -= 1
                ctx.board = ctx.record.get_board(ctx.record_index)
    elif ctx.status == Status.FINISH:
        if event.key == pygame.K_i:
            ctx.board = client.get_init_board()
            ctx.status = Status.PAUSE
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


def handle_event(ctx: Context, event: pygame.event.Event):
    if event.type == pygame.MOUSEBUTTONDOWN:
        mouse_pos = pygame.mouse.get_pos()
        x, y = ChessBoard.pixi_to_chess(mouse_pos[0], mouse_pos[1])
        if 0 <= x <= 10 and 0 <= y <= 9:
            handle_chess_board_mouse_button_down(ctx, event, x, y)
    if event.type == pygame.KEYDOWN:
        handle_key_down(ctx, event)
    if event.type == pygame.QUIT:
        client.engine_stop()
        ctx.running = False
    if event.type == pygame.USEREVENT:
        if event.user_type == pygame_gui.UI_FILE_DIALOG_PATH_PICKED:
            if event.text:
                try:
                    with open(event.text, 'r') as file:
                        content = file.read()
                        ctx.record = ChessRecord()
                        ctx.record.from_json(content)
                        ctx.record_index = 0
                        ctx.board = ctx.record.get_board(0)
                except Exception as e:
                    print(e)


last_refresh_think_text_time = 0


def refresh_think_text(ctx: Context):
    global last_refresh_think_text_time
    current_time = time.time()
    time_since_last_call = current_time - last_refresh_think_text_time
    if time_since_last_call < 5:
        return
    last_refresh_think_text_time = current_time
    ctx.think_text = client.best_path() + "\n" + ctx.think_text


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

ctx.board = client.get_init_board()["board"]

while ctx.running:
    for event in pygame.event.get():
        print(event)
        gui_manager.process_events(event)
        handle_event(ctx, event)

    screen.fill((255, 255, 255))
    gui_manager.update(pygame.time.Clock().tick(60) / 1000.0)
    ctx.board.draw(screen)
    if ctx.choose_pos is not None:
        ctx.board.highlight(screen, ctx.choose_pos[0], ctx.choose_pos[1], (255, 0, 0))
    think_text_box.set_text(ctx.think_text)
    help_text_box.set_text(ctx.help_text)
    if ctx.status == Status.SEARCHING:
        refresh_think_text(ctx)
    gui_manager.draw_ui(screen)
    pygame.display.flip()
    pygame.display.update()

pygame.quit()
