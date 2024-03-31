from chess_board import *


class Context:
    def __init__(self):
        self.running = True
        self.board = ChessBoard()
        self.think_text = "炮二平五 炮8平5 马二进三 马8进7 车一进一 车9平8 车一平六 车8进6 车六进七 马2进1 车九进一 炮2进7"
