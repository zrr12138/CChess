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
#include "WeightedRandomSelector.h"

using namespace CChess;

void test1() {
    ChessBoard board;
    //board.BoardRed(true);
    board.initBoard();
    auto &engine = MCTSEngine::getInstance();

    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 100000;
    int black = 0;
    int red = 0;
    int He = 0;
    for (int i = 0; i < n; i++) {
        auto end = engine.Simulation(board, true);
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

void test2() {
    ChessBoard board;
    board.initBoard();
    auto &engine = MCTSEngine::getInstance();
    engine.StartSearch(board, true);
    int step = 50;
    while (board.End() == BoardResult::NOT_END) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto move = engine.GetResult();
        std::string move_string;
        assert(board.MoveConversion(move, &move_string));
        std::cout << "move:" << move << " " << move_string << std::endl;
        engine.Action(move);
        assert(board.Move(move));
        if (step-- < 0) {
            break;
        }
    }
    engine.Stop();
}

void test3() {
    ChessBoard board;
    auto &engine = MCTSEngine::getInstance();
    board.initBoard();
    engine.StartSearch(board, true);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    engine.Stop();
    engine.StartSearch(board, true);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    engine.Stop();

}

void test4() {
    auto &engine = MCTSEngine::getInstance();
    {
        ChessBoard board;
        board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
        board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
        board.SetChessAt(Chess(ChessType::Bing, true), 1, 3);
        board.SetChessAt(Chess(ChessType::Bing, true), 1, 5);
        board.SetChessAt(Chess(ChessType::Shi, false), 2, 3);
        board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
        board.SetChessAt(Chess(ChessType::Bing, true), 3, 4);
        engine.StartSearch(board, true);
    }
    std::vector<std::pair<ChessMove, double>> path;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    engine.GetBestPath(&path);
    auto board = engine.GetChessBoard();
    for (auto &it: path) {
        std::string move_str;
        assert(board.MoveConversion(it.first, &move_str));
        assert(board.Move(it.first));
        std::cout << move_str << "(" << it.second << ")" << "-->";
    }
    std::cout << std::endl;
    engine.Stop();

}

void test5() {
    const int size = 5;
    int a[] = {1, 2, 3, 4, 5};
    int sum = std::accumulate(a, a + size, 0);
    int cnt[size] = {0};
    WeightedRandomSelector selector(a, 5);

    int n = 100000000;
    auto start = common::TimeUtility::CLockRealTimeMs();
    for (int i = 0; i < n; i++) {
        cnt[selector.getRandomIndex()]++;
    }
    std::cout << "pick " << n << " cost " << common::TimeUtility::CLockRealTimeMs() - start << " ms" << std::endl;
    for (int i = 0; i < size; i++) {
        double probability = static_cast<double>(cnt[i]) / n;
        std::cout << "Probability of index " << i << ": " << probability << std::endl;
        assert(std::abs(probability - static_cast<double >(a[i]) / sum) < 0.001);
    }
    std::cout << "change a[1] to 100" << std::endl;
    a[1] = 100;
    selector.UpdateWeight(1, 100);
    sum = std::accumulate(a, a + size, 0);
    memset(cnt, 0, sizeof(cnt));
    for (int i = 0; i < n; i++) {
        cnt[selector.getRandomIndex()]++;
    }
    for (int i = 0; i < size; i++) {
        double probability = static_cast<double>(cnt[i]) / n;
        std::cout << "Probability of index " << i << ": " << probability << std::endl;
        assert(std::abs(probability - static_cast<double >(a[i]) / sum) < 0.001);
    }

    start = common::TimeUtility::CLockRealTimeMs();
    for (int i = 0; i < n; i++) {
        selector.UpdateWeight(0, i);
    }
    std::cout << "update weight " << n << " cost " << common::TimeUtility::CLockRealTimeMs() - start << " ms"
              << std::endl;

}

void engine_auto_action(ChessBoard board, bool is_red) {
    auto &engine = MCTSEngine::getInstance();
    engine.StartSearch(board, is_red);
    int step = 50;
    while (board.End() == BoardResult::NOT_END) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        auto move = engine.GetResult();
        std::string move_string;
        assert(board.MoveConversion(move, &move_string));
        std::cout << "move:" << move << " " << move_string << std::endl;
        engine.Action(move);
        assert(board.Move(move));
        if (step-- < 0) {
            break;
        }
    }
    engine.Stop();
}

void test6() {
    ChessBoard board;
    board.ParseFromString(
            R"([{"type": 2, "is_red": true, "row": 0, "col": 3}, {"type": 0, "is_red": false, "row": 0, "col": 5}, {"type": 2, "is_red": true, "row": 1, "col": 4}, {"type": 2, "is_red": true, "row": 2, "col": 5}, {"type": 4, "is_red": false, "row": 8, "col": 0}, {"type": 0, "is_red": true, "row": 9, "col": 4}])");
    engine_auto_action(board, false);
}
void test7(){
    ChessBoard board;
    board.ParseFromString(R"([{"type": 4, "is_red": true, "row": 0, "col": 4}, {"type": 5, "is_red": true, "row": 0, "col": 5}, {"type": 0, "is_red": false, "row": 1, "col": 5}, {"type": 4, "is_red": false, "row": 7, "col": 6}, {"type": 0, "is_red": true, "row": 9, "col": 4}])");
    auto &engine = MCTSEngine::getInstance();
    assert(engine.StartSearch(board, false));
    std::this_thread::sleep_for(std::chrono::seconds(60));
    std::cout<<"10 seconds root n"<<engine.GetRootN()<<std::endl;
    NodeHashManager::getInstance().Dump();
    engine.Stop();
}
int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir = ".";
    FLAGS_minloglevel = 1;

//    test1();
//    test2();
//    test3();
//    test4();
//    test5();
//    test6();
    test7();
}
