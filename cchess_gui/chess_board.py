import os
from enum import IntEnum
import pygame
from surface_cache import SurfaceCache


class ChessType(IntEnum):
    Empty = -1
    Wang = 0
    Ma = 1
    Bing = 2
    Shi = 3
    Ju = 4
    Pao = 5
    Xiang = 6


class ChessMove:
    def __init__(self, start_x, start_y, end_x, end_y):
        self.start_x = start_x
        self.start_y = start_y
        self.end_x = end_x
        self.end_y = end_y

    def __eq__(self, rhs):
        return isinstance(rhs, ChessMove) and \
            self.start_x == rhs.start_x and \
            self.start_y == rhs.start_y and \
            self.end_x == rhs.end_x and \
            self.end_y == rhs.end_y

    def __ne__(self, rhs):
        return not self.__eq__(rhs)


class BoardResult(IntEnum):
    RED_WIN = 0
    BLACK_WIN = 1
    NOT_END = 2


class Chess:
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
        bias = 27
        begin_x = 70
        begin_y = 70
        rect.x = begin_x + 54 * col - bias
        rect.y = begin_y + 54 * row - bias
        surface.blit(image, rect)


class ChessBoard:
    def __init__(self):
        self.board = []
        for i in range(10):
            temp = []
            for j in range(9):
                temp.append(Chess(ChessType.Empty, False))
            self.board.append(temp)

    def draw(self, surface):
        for i in range(10):
            for j in range(9):
                chess = self.board[i][j]
                chess.draw(surface, i, j)

    def get_chess(self, row, col):
        return self.board[row][col]

    def set_chess(self, row, col, chess):
        self.board[row][col] = chess

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
        begin_x = 70
        begin_y = 70
        cell_size = 54
        bias = cell_size // 2
        col = (pixel_x - begin_x + bias) // cell_size
        row = (pixel_y - begin_y + bias) // cell_size
        return row, col
