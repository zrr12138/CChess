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


class ChessSprite(pygame.sprite.Sprite):

    # Constructor. Pass in the color of the block,
    # and its x and y position
    def __init__(self, chess: Chess, row, col):
        assert chess.type != ChessType.Empty
        # Call the parent class (Sprite) constructor
        pygame.sprite.Sprite.__init__(self)

        self.image = SurfaceCache().load(chess.get_image_file_name())
        bias = 27
        begin_x = 70
        begin_y = 70
        self.rect = self.image.get_rect()
        self.rect.x = begin_x + 54 * col - bias
        self.rect.y = begin_y + 54 * row - bias

        mask = pygame.mask.from_surface(self.image)
        outline = mask.outline()
        pygame.draw.polygon(self.image, (217, 175, 111, 0), outline, 7)


class ChessBoard:
    def __init__(self):
        self.board = []
        for i in range(10):
            temp = []
            for j in range(9):
                temp.append(Chess(ChessType.Empty, False))
            self.board.append(temp)

    def to_sprite_group(self):
        chess_group = pygame.sprite.Group()
        for i in range(10):
            for j in range(9):
                chess = self.board[i][j]
                if chess.type != ChessType.Empty:
                    # noinspection PyTypeChecker
                    chess_group.add(ChessSprite(chess, i, j))
        return chess_group
