import os
import subprocess
import time
import sys
import shutil
from client import Client


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


# Get the directory of the current script
script_dir = os.path.dirname(os.path.realpath(__file__))

# Get the current working directory
current_dir = os.getcwd()

# Check if the current working directory matches the script directory
if current_dir != script_dir:
    print("The current working directory does not match the directory of the script!")
    print("Please change the working directory to the script directory:", script_dir)
    exit()

shutil.rmtree("CChess", ignore_errors=True)
run_shell_print("git clone git@github.com:zrr12138/CChess.git")

shutil.rmtree("binary", ignore_errors=True)
os.mkdir("binary")
with open("engine_list", "r") as f:
    for line in f.readlines():
        git_version = line.strip()
        if len(git_version) == 0:
            continue
        run_shell_print(f"cd CChess && git checkout {git_version}")
        run_shell_print("cd CChess/engine && bash build.sh")
        run_shell_print(f"cp CChess/engine/build/server binary/{git_version}")
