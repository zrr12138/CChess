from chess_board import *
import requests
from logger import logger
import copy


class Client:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port

    def get_init_board(self):
        url = f"http://{self.ip}:{self.port}/init_board"
        res = requests.get(url).json()
        logger.debug(f"access {url} get request {res}")
        assert res["error"] == 0
        chess_board = ChessBoard()
        chess_board.parse_from_string(json.dumps(res["board"]))
        return copy.deepcopy(chess_board)

    def board_move(self, board: ChessBoard, move: Move):
        url = f"http://{self.ip}:{self.port}/board_move"
        data = json.dumps({"move": move.to_string(), "board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        assert err != 1
        if err == 0:
            board.parse_from_string(json.dumps(res["board"]))
            return board
        else:
            logger.error(f"move failed, get error:{err}")
            return board
