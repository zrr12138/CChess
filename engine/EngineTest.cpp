//
// Created by zrr on 24-3-24.
//
#include "ChessBoard.h"
#include <vector>
#include  <assert.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <chrono>
#include <common/timeutility.h>
#include "MCTSEngine.h"
#include <random>
#include "gflags/gflags.h"
#include "glog/logging.h"
using namespace CChess;

void test1() {
    ChessBoard board;
    //board.BoardRed(true);
    board.initBoard();
    MCTSEngine engine(1);

    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 100000;
    BoardResult end;
    int black = 0;
    int red = 0;
    int He = 0;
    for (int i = 0; i < n; i++) {
        engine.Simulation(board, true, &end);
        if (end == BoardResult::RED_WIN)
            red++;
        else if (end == BoardResult::BLACK_WIN)
            black++;
        else
            He++;
    }
    std::cout << "红棋win: " << red << " 黑棋win: " << black << " 和棋: " << He << std::endl;
    std::cout << "simulation " << n << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}

int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging("ManualTest");
    FLAGS_log_dir = ".";
    FLAGS_v = 2;

    test1();

}
