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
    int n = 1000;
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

void test2(int thread_num) {
    ChessBoard board;
    board.initBoard();
    MCTSEngine engine(thread_num);
    engine.StartSearch(board, true);
    while (board.End() == BoardResult::NOT_END) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        auto move = engine.GetResult();
        std::string move_string;
        assert(board.MoveConversion(move, &move_string));
        std::cout << "move:" << move << " " << move_string << std::endl;
        engine.Action(move);
        assert(board.Move(move));
    }
    engine.Stop();
}
void test3(){
    ChessBoard board;
    MCTSEngine engine(1);
    board.initBoard();
    engine.StartSearch(board,true);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    engine.Stop();
    engine.StartSearch(board,true);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    engine.Stop();

}
void test4() {
    MCTSEngine engine(8);

    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Bing, true), 1, 3);
    board.SetChessAt(Chess(ChessType::Bing, true), 1, 5);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
    board.SetChessAt(Chess(ChessType::Bing, true), 3, 4);

    engine.StartSearch(board, true);

    std::vector<std::pair<ChessMove, double>> path;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    engine.GetBestPath(&path);
    auto &new_board = const_cast<ChessBoard &>(engine.GetChessBoard());
    for(auto &it:path){
        std::string move_str;
        assert(engine.GetChessBoard().MoveConversion(it.first,&move_str));
        board.Move(it.first);
        new_board = board;
        std::cout<<move_str<<"("<<it.second<<")"<<"-->";
    }
    std::cout<<std::endl;
    engine.Stop();

}
int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging("ManualTest");
    FLAGS_log_dir = ".";
    FLAGS_v = 2;

    //test1();
    //test2(1);
    //test3();
    test4();
}
