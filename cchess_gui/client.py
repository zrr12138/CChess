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
        result = dict()
        result["error"] = res["error"]
        if result["error"] == 0:
            board1 = ChessBoard()
            board1.parse_from_string(json.dumps(res["board"]))
            result["board"] = board1
        return result

    def start_search(self, board: ChessBoard, red_first: bool):
        url = f"http://{self.ip}:{self.port}/start_search"
        data = json.dumps({"board": board.to_string(), "red_first": red_first})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        if res["error"] != 0:
            logger.error(f"start_search failed, get error:{err}")
        return res

    def engine_action(self, board: ChessBoard):
        url = f"http://{self.ip}:{self.port}/engine_action"
        data = json.dumps({"board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} get request {res}")
        err = res["error"]
        result = dict()
        result["error"] = err
        if err == 0:
            board1 = ChessBoard()
            board1.parse_from_string(json.dumps(res["board"]))
            move = Move(0, 0, 0, 0)
            move.parse_from_string(json.dumps(res["move"]))
            result["board"] = board1
            result["move"] = move
        else:
            logger.error(f"engine_action failed, get error:{err}")
        return result

    def engine_stop(self):
        url = f"http://{self.ip}:{self.port}/engine_stop"
        res = requests.get(url).json()
        logger.debug(f"access {url} get request {res}")
        err = res["error"]
        if res["error"] != 0:
            logger.error(f"start_search failed, get error:{err}")
        return res

    def reverse_board(self, board: ChessBoard):
        url = f"http://{self.ip}:{self.port}/reverse_board"
        data = json.dumps({"board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        result = dict()
        result["error"] = err
        if err == 0:
            board1 = ChessBoard()
            board1.parse_from_string(json.dumps(res["board"]))
            result["board"] = board1
        else:
            logger.error(f"move failed, get error:{err}")
        return result

    def best_path(self):
        url = f"http://{self.ip}:{self.port}/best_path"
        res = requests.get(url).json()
        logger.debug(f"access {url} get request {res}")
        err = res["error"]
        result = dict()
        result["error"] = err
        if err == 0:
            result["best_path"] = res["best_path"]
        else:
            logger.error(f"get best_path failed, get error:{err}")
        return result

    def board_end(self, board: ChessBoard):
        url = f"http://{self.ip}:{self.port}/board_end"
        data = json.dumps({"board": board.to_string()})
        res = requests.post(url, data=data).json()
        logger.debug(f"access {url} data= {data} get request {res}")
        err = res["error"]
        result = dict()
        result["error"] = err
        if err == 0:
            result["end"] = BoardResult(res["end"])
        else:
            logger.error(f"get board end failed, get error:{err}")
        return result
