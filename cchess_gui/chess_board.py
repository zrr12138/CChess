import os
from enum import IntEnum
import pygame
from surface_cache import SurfaceCache
import copy
import json


class ChessType(IntEnum):
    Empty = -1
    Wang = 0
    Ma = 1
    Bing = 2
    Shi = 3
    Ju = 4
    Pao = 5
    Xiang = 6


class BoardResult(IntEnum):
    RED_WIN = 0
    BLACK_WIN = 1
    NOT_END = 2


class Chess:
    begin_x = 70
    begin_y = 70
    cell_size = 54
    bias = cell_size // 2

    def __init__(self, type: ChessType, is_red):
        self.type = type
        self.is_red = is_red

    def get_image_file_name(self):
        if self.is_red:
            color = "red"
        else:
            color = "black"
        types = ["wang", "ma", "bing", "shi", "ju", "pao", "xiang"]
        type_str = types[self.type.value]
        return os.path.join("image", f"{color}-{type_str}.png")

    def draw(self, surface, row, col):
        if self.type == ChessType.Empty:
            return
        image = SurfaceCache().load(self.get_image_file_name())
        rect = image.get_rect()
        rect.x = Chess.begin_x + Chess.cell_size * col - Chess.bias
        rect.y = Chess.begin_y + Chess.cell_size * row - Chess.bias
        surface.blit(image, rect)


class ChessBoard:
    def __init__(self):
        self.board = []
        for i in range(10):
            temp = []
            for j in range(9):
                temp.append(Chess(ChessType.Empty, False))
            self.board.append(temp)
        self.image = pygame.image.load(os.path.join("image", "chessboardafter.png"))
        self.rect = self.image.get_rect()

    def draw(self, surface):
        board = self.image.copy()
        for i in range(10):
            for j in range(9):
                chess = self.board[i][j]
                chess.draw(board, i, j)
        surface.blit(board, pygame.Rect(0, 0, self.rect.width, self.rect.height))

    def highlight(self, surface, row, col, color):
        rect = pygame.Rect(0, 0, Chess.cell_size, Chess.cell_size)
        rect.x = Chess.begin_x + Chess.cell_size * col - Chess.bias
        rect.y = Chess.begin_y + Chess.cell_size * row - Chess.bias
        pygame.draw.rect(surface, color, rect, 5)

    def get_chess(self, row, col):
        return copy.deepcopy(self.board[row][col])

    def set_chess(self, row, col, chess):
        self.board[row][col] = copy.deepcopy(chess)

    def clear_chess(self, row, col):
        self.board[row][col].type = ChessType.Empty

    def __contains__(self, chess):
        for i in range(10):
            for j in range(9):
                if self.board[i][j] == chess:
                    return True
        return False

    def __iter__(self):
        for i in range(10):
            for j in range(9):
                yield i, j, self.board[i][j]

    @staticmethod
    def pixi_to_chess(pixel_x, pixel_y):
        col = (pixel_x - Chess.begin_x + Chess.bias) // Chess.cell_size
        row = (pixel_y - Chess.begin_y + Chess.bias) // Chess.cell_size
        return row, col

    def clear_board(self):
        for row in range(10):
            for col in range(9):
                self.board[row][col] = Chess(ChessType.Empty, False)

    def parse_from_string(self, string):
        json_board = json.loads(string)
        self.clear_board()
        for jsonChess in json_board:
            row = jsonChess["row"]
            col = jsonChess["col"]
            chess_type = ChessType(jsonChess["type"])
            is_red = jsonChess["is_red"]
            assert chess_type != ChessType.Empty
            assert 0 <= row < 10
            assert 0 <= col < 9
            chess = Chess(chess_type, is_red)
            self.set_chess(row, col, chess)

    def to_string(self):
        json_board = []
        for row in range(10):
            for col in range(9):
                chess = self.board[row][col]
                if chess.type == ChessType.Empty:
                    continue
                json_chess = {
                    "type": chess.type,
                    "is_red": chess.is_red,
                    "row": row,
                    "col": col
                }
                json_board.append(json_chess)

        return json.dumps(json_board)


class Move:
    def __init__(self, start_x, start_y, end_x, end_y):
        self.start_x = start_x
        self.start_y = start_y
        self.end_x = end_x
        self.end_y = end_y

    def __le__(self, rhs):
        return self.start_x <= rhs.start_x and self.start_y <= rhs.start_y and self.end_x <= rhs.end_x and self.end_y <= rhs.end_y

    def to_string(self):
        json_move = {
            "start_x": self.start_x,
            "start_y": self.start_y,
            "end_x": self.end_x,
            "end_y": self.end_y
        }
        return json.dumps(json_move)

    def parse_from_string(self, string):
        json_move = json.loads(string)
        self.start_x = json_move["start_x"]
        self.start_y = json_move["start_y"]
        self.end_x = json_move["end_x"]
        self.end_y = json_move["end_y"]
