import os
import subprocess
import time
import sys
import shutil
from client import Client
from collections import namedtuple
from chess_board import *
from chess_tournament import ChessTournament


def run_shell_print(args, try_num: int = 1, retry_interval: int = 3, continue_on_error: bool = False,
                    return_result: bool = False):
    while try_num > 0:
        print(args)
        assert isinstance(args, list) == False
        process = subprocess.Popen(
            args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
        result = ""
        for line in iter(process.stdout.readline, b''):
            print(line.decode(), end='')
            result += line.decode()
        return_code = process.wait()
        process.stdout.close()
        print()
        sys.stdout.flush()
        if return_code:
            try_num -= 1
            if try_num == 0:
                break
            time.sleep(retry_interval)
            print("retry exec shell command:")
            continue
        else:
            break
    if return_code and not continue_on_error:
        print(f"shell command return error code: {return_code}")
        sys.exit(-1)
    if return_result:
        return result


TEST_PATH = "/home/zrr/cchess_test"
CCHESS_PATH = os.path.join(TEST_PATH, "CChess")
BINARY_PATH=os.path.join(TEST_PATH,"binary")
script_dir = os.path.dirname(os.path.realpath(__file__))

if not os.path.exists(TEST_PATH):
    print("test path not exist")
    sys.exit(-1)

os.chdir(TEST_PATH)

if not os.path.exists(CCHESS_PATH):
    run_shell_print("git clone git@github.com:zrr12138/CChess.git")
else:
    run_shell_print("cd CChess && git fetch --all")


if not os.path.exists(BINARY_PATH):
    os.mkdir(BINARY_PATH)

binary_set = os.listdir(BINARY_PATH)
run_shell_print(f"cp {script_dir}/engine_list {TEST_PATH}")
with open("engine_list", "r") as f:
    for line in f.readlines():
        git_version = line.strip()
        if len(git_version) == 0:
            continue
        name = git_version[0:10]
        if name in binary_set:
            continue
        run_shell_print(f"cd CChess && git checkout {git_version}")
        run_shell_print("cd CChess/engine && bash build.sh")
        run_shell_print(f"cp CChess/engine/build/server {BINARY_PATH}/{name}")
        binary_set.append(name)

print("binary generate finish!")
print(binary_set)


def engine_fight(contest):
    assert isinstance(contest, Contest)
    board = ChessBoard()

    board = red_client.get_init_board()

    red_client.start_search(board, True)
    black_client.start_search(board, False)

    red_turn = True
    turn_cnt = 0
    while red_client.board_end(board) == BoardResult.NOT_END:
        turn_cnt += 1
        if turn_cnt > 400:
            return BoardResult.NOT_END
        time.sleep(contest.think_time)
        if red_turn:
            temp = copy.deepcopy(board)
            board, move = red_client.engine_action(board)
            black_client.board_move(temp, move)
        else:
            temp = copy.deepcopy(board)
            board, move = black_client.engine_action(board)
            red_client.board_move(temp, move)
        red_turn = not red_turn
    return red_client.board_end(board)


port1 = 12138
port2 = 12138 + 1

Contest = namedtuple('Contest', ['thread_num', 'think_time', 'repeat'])
red_client = Client("127.0.0.1", port1)
black_client = Client("127.0.0.1", port2)
contests = []
contests.append(Contest(4, 5, 3))
final_result = ChessTournament(binary_set)

for contest in contests:
    assert isinstance(contest, Contest)
    print(contest)
    result = ChessTournament(binary_set)
    for ii in range(contest.repeat):
        for binary1 in binary_set:
            for binary2 in binary_set:
                run_shell_print(
                    f"nohup ./binary/{binary1} --thread_num {contest.thread_num} --port {port1} > {binary1}.log 2>&1 &")
                run_shell_print(
                    f"nohup ./binary/{binary2} --thread_num {contest.thread_num} --port {port2} > {binary2}.log 2>&1 &")
                end = engine_fight(contest)
                result.record_result(binary1, binary2, end)
                final_result.record_result(binary1, binary2, end)
                run_shell_print(f"pkill -f {binary1}")
                run_shell_print(f"pkill -f {binary2}")
    result.calculate_scores()
    result.print_table()

final_result.calculate_scores()
final_result.print_table()
