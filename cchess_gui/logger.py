
import logging
from logging.handlers import RotatingFileHandler
import os
import sys
def get_logger():
    logging.getLogger().handlers = []  # 去除默认的 root logger 配置
    script_name = "cchess_gui"
    log_dir = "./"
    log_file_path = os.path.join(log_dir, script_name + ".log")

    logger = logging.getLogger(script_name)
    log_level = logging.DEBUG
    logger.setLevel(log_level)
    formatter = logging.Formatter("%(asctime)s %(filename)s:%(lineno)d %(funcName)s %(levelname)s %(message)s")

    rfh = RotatingFileHandler(log_file_path, maxBytes=1024 * 1024 * 100, backupCount=1, encoding="utf-8")
    rfh.setLevel(log_level)
    rfh.setFormatter(formatter)
    logger.addHandler(rfh)

    # console_handler = logging.StreamHandler()
    # console_handler.setLevel(log_level)
    # console_handler.setFormatter(formatter)
    # logger.addHandler(console_handler)
    return logger


logger = get_logger()
