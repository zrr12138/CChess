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
        chess_board = ChessBoard()
        chess_board.parse_from_string(json.dumps(res["board"]))
        return {"board": copy.deepcopy(chess_board), "error": res["error"]}

    def board_move(self, board: ChessBoard, move: Move):
        url = f"http://{self.ip}:{self.port}/board_move"
        data = json.dumps({"move": move.to_string(), "board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        if err == 0:
            board.parse_from_string(json.dumps(res["board"]))
            return board
        else:
            logger.error(f"move failed, get error:{err}")
            return board

    def start_search(self, board: ChessBoard, red_first: bool):
        url = f"http://{self.ip}:{self.port}/start_search"
        data = json.dumps({"board": board.to_string(), "red_first": red_first})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        if err == 0:
            return True
        else:
            logger.error(f"start_search failed, get error:{err}")
            return False

    def engine_action(self, board: ChessBoard):
        url = f"http://{self.ip}:{self.port}/engine_action"
        data = json.dumps({"board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} get request {res}")
        err = res["error"]
        if err == 0:
            board.parse_from_string(json.dumps(res["board"]))
            move = Move(0, 0, 0, 0)
            move.parse_from_string(json.dumps(res["move"]))
            return board, move
        else:
            logger.error(f"engine_action failed, get error:{err}")
            return board, Move(0, 0, 0, 0)

    def engine_stop(self):
        url = f"http://{self.ip}:{self.port}/engine_stop"
        res = requests.get(url).json()
        logger.debug(f"access {url} get request {res}")
        err = res["error"]
        if err == 0:
            return True
        else:
            logger.error(f"engine_stop failed, get error:{err}")
            return False

    def reverse_board(self, board: ChessBoard):
        url = f"http://{self.ip}:{self.port}/reverse_board"
        data = json.dumps({"board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        if err == 0:
            board.parse_from_string(json.dumps(res["board"]))
            return board
        else:
            logger.error(f"move failed, get error:{err}")
            assert False

    def best_path(self):
        url = f"http://{self.ip}:{self.port}/best_path"
        res = requests.get(url).json()
        logger.debug(f"access {url} get request {res}")
        err = res["error"]
        if err == 0:
            return res["best_path"]
        else:
            logger.error(f"get best_path failed, get error:{err}")
            assert False

    def board_end(self, board: ChessBoard):
        url = f"http://{self.ip}:{self.port}/board_end"
        data = json.dumps({"board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        if err == 0:
            return BoardResult(res["end"])
        else:
            logger.error(f"get board end failed, get error:{err}")
            assert False
