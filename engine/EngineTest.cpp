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

#include "gflags/gflags.h"
#include "glog/logging.h"
using namespace CChess;

void test1() {

    ChessBoard board;
    board.initBoard();
    MCTSEngine engine(1);

    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 1000000;
    for (int i = 0; i < n; i++) {
        engine.Simulation(board, true);
    }
    std::cout << "simulation " << n << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}

int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging("ManualTest");
    FLAGS_log_dir = ".";
    FLAGS_v = 2;

    test1();

}
