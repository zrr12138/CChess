from chess_board import *


class Status(IntEnum):
    SEARCHING = 1
    PAUSE = 2
    PLACE = 3
    FINISH = 4


def get_help_text(status: Status):
    return "s-开始搜索 d-结束搜索 i-初始棋盘 r-棋盘翻转 m-立即出子 p-放置模式"


class Context:
    def __init__(self):
        self.running = True
        self.board = ChessBoard()
        self.think_text = "炮二平五 炮8平5 马二进三 马8进7 车一进一 车9平8 车一平六 车8进6 车六进七 马2进1 车九进一 炮2进7"
        self.status = Status.PAUSE
        self.human_red = True
        self.place_red = True
        self.choose_pos = None
        self.record=None
        self.record_index=0

    @property
    def help_text(self):
        if self.status == Status.SEARCHING:
            return "d-停止搜索 m-引擎出招"
        elif self.status == Status.PAUSE:
            return "s-开始搜索(红先) ctrl+s-开始搜索(黑先) r-棋盘翻转 p-放置模式\n i-初始化棋盘 l-加载棋谱  <-- 上一步  -->下一步"
        elif self.status == Status.PLACE:
            return "w-王 m-马 b-兵 s-士 j-车 p-炮 x-象 tab切换红黑 右键删除棋子 d-结束放置"
        elif self.status == Status.FINISH:
            return "i-初始化棋盘"
