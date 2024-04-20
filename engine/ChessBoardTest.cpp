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
#include "common/timeutility.h"
#include <chrono>

using namespace CChess;


void test1() {
    ChessBoard board;
    board.ClearBoard();
    /*board.BoardRed(true);
    board.SetChessAt(Chess(ChessType::Ma, true), 2, 0);
    board.SetChessAt(Chess(ChessType::Xiang, false), 0, 1);
    board.SetChessAt(Chess(ChessType::Bing, true), 2, 1);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Ju, true), 0, 8);*/
    board.initBoard();
    std::vector<ChessMove> moves;
    ChessMove move{};
    std::vector<Chess> dead;
    std::string move_string;
    /*board.GetMovesFrom(2, 0, &moves);
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 0, 1)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 4, 1)) != moves.end());
    */
    int n = 100;
    bool is_red_now = true;
    while (n--) {
        move = board.RandMove2(is_red_now);
        board.MoveConversion(ChessMove(move), &move_string);
        std::cout << move << std::endl;
        std::cout << move_string << std::endl;
        assert(board.Move(ChessMove(move)));
        // 如果错误，则有些地方move不合法
        is_red_now = !is_red_now;
        auto end = board.End();
        std::cout << getBoardResultStr(end) << std::endl;
        if (end == BoardResult::BLACK_WIN) {
            assert(board.EvaluatePosition() < 0);
            break;
        } else if (end == BoardResult::RED_WIN) {
            assert(board.EvaluatePosition() > 0);
            break;
        }
    }
    std::string err;
    assert(board.IsLegal(&err));
    board.GetDeadChess(&dead);
}

void test2() {
    ChessBoard board;
    board.ClearBoard();
    board.initBoard();
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    ChessMove move;
    std::string err;
    move = board.RandMove2(false);
    board.GetMovesFrom(move.start_x, move.start_y, &moves);
    assert(moves.size() == 1 || moves.size() == 2 || moves.size() == 12);
    move = board.RandMove2(true);
    assert(moves.size() == 1 || moves.size() == 2 || moves.size() == 12);
    assert(board.Move(ChessMove(9, 0, 6, 0)) == false);
    assert(board.Move(ChessMove(9, 0, 5, 0)) == false);
    assert(board.Move(ChessMove(9, 1, 8, 3)) == false);
    assert(board.Move(ChessMove(0, 7, 1, 5)) == false);
    //board.Move(ChessMove(9, 0, 7, 0));
    assert(board.IsLegal(&err));
}

void test3() {
    ChessBoard board(false);
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 0);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 2);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 4);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 6);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 8);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 0);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 2);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 4);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 6);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 8);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMoves(true, &moves);
    assert(moves.size() == 5);
    moves.clear();
    board.GetMoves(false, &moves);
    assert(moves.size() == 5);
    moves.clear();
    board.GetMovesFrom(6, 8, &moves);
    assert(moves.size() == 1);
    moves.clear();
    board.Move(ChessMove(3, 0, 4, 0));
    board.Move(ChessMove(6, 0, 5, 0));
    assert(board.Move(ChessMove(5, 0, 5, 1)) == false);
    assert(board.Move(ChessMove(4, 0, 5, 0)));
    board.GetMovesFrom(5, 0, &moves);
    assert(moves.size() == 2);
}

void test4() {
    ChessBoard board(true);
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 0);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 2);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 4);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 6);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 8);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 0);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 2);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 4);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 6);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 8);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMoves(true, &moves);
    assert(moves.size() == 13);
    moves.clear();
    board.GetMoves(false, &moves);
    assert(moves.size() == 13);
    moves.clear();
    board.GetMovesFrom(6, 6, &moves);
    assert(moves.size() == 3);
    moves.clear();
    board.GetMovesFrom(6, 8, &moves);
    assert(moves.size() == 2);
    moves.clear();
    board.SetChessAt(Chess(ChessType::Ma, true), 7, 4);
    board.GetMovesFrom(6, 4, &moves);
    assert(moves.size() == 2);
}

void test5() {
    ChessBoard board(true);
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Xiang, false), 7, 4);
    board.SetChessAt(Chess(ChessType::Xiang, true), 0, 2);
    board.SetChessAt(Chess(ChessType::Bing, true), 9, 7);
    board.SetChessAt(Chess(ChessType::Bing, true), 8, 3);
    board.SetChessAt(Chess(ChessType::Ma, true), 8, 4);
    board.SetChessAt(Chess(ChessType::Ma, true), 8, 5);
    board.SetChessAt(Chess(ChessType::Shi, false), 9, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 7, 5);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMovesFrom(7, 4, &moves);
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), ChessMove(7, 4, 5, 2)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(7, 4, 5, 6)) != moves.end());
    moves.clear();
    board.GetMovesFrom(8, 4, &moves);
    assert(moves.size() == 0);
    moves.clear();
    board.GetMovesFrom(8, 5, &moves);
    assert(moves.size() == 1);
    moves.clear();
    board.GetMovesFrom(9, 3, &moves);
    assert(moves.size() == 1);
    moves.clear();
    board.GetMovesFrom(9, 7, &moves);
    assert(moves.size() == 2);
    moves.clear();
}

void test6() {
    ChessBoard board;
    board.initBoard();
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    std::string QiPu;
    board.GetMovesFrom(7, 7, &moves);
    assert(moves.size() == 12);
    moves.clear();
    board.Move(ChessMove(7, 7, 6, 7));
    board.GetMovesFrom(6, 7, &moves);
    assert(moves.size() == 6);
    moves.clear();
    board.MoveConversion(ChessMove(7, 1, 0, 1), &QiPu);
    board.Move(ChessMove(7, 1, 0, 1));
    assert(QiPu == "炮八进七");
    board.MoveConversion(ChessMove(0, 1, 0, 3), &QiPu);
    board.Move(ChessMove(0, 1, 0, 3));
    assert(QiPu == "炮八平六");
    board.MoveConversion(ChessMove(2, 7, 9, 7), &QiPu);
    board.Move(ChessMove(2, 7, 9, 7));
    assert(QiPu == "炮8进7");
    board.initBoard();
    board.MoveConversion(ChessMove(7, 1, 8, 1), &QiPu);
    board.Move(ChessMove(7, 1, 8, 1));
    assert(QiPu == "炮八退一");
    board.MoveConversion(ChessMove(8, 1, 8, 7), &QiPu);
    board.Move(ChessMove(8, 1, 8, 7));
    assert(QiPu == "炮八平二");
    board.MoveConversion(ChessMove(7, 7, 6, 7), &QiPu);
    board.Move(ChessMove(7, 7, 6, 7));
    assert(QiPu == "前炮进一");
    board.MoveConversion(ChessMove(8, 7, 2, 7), &QiPu);
    board.Move(ChessMove(8, 7, 2, 7));
    assert(QiPu == "后炮进六");
}

void test7() {
    ChessBoard board;
    board.ClearBoard();
    board.initBoard();
    std::string errorMessage;
    ChessMove move;
    std::string QiPu;
    std::vector<Chess> dead;
    board.Move(ChessMove(7, 1, 0, 1));
    board.Move(ChessMove(0, 1, 0, 3));
    board.Move(ChessMove(0, 3, 0, 5));
    board.Move(ChessMove(0, 5, 0, 2));
    board.Move(ChessMove(0, 2, 0, 6));
    board.Move(ChessMove(0, 6, 0, 0));
    board.Move(ChessMove(0, 0, 0, 7));
    assert(board.IsLegal(&errorMessage) == true);
    board.GetDeadChess(&dead);
    assert(dead.size() == 7);
    /*for (const Chess dead1: dead) {
        std::cout << "Type: " << dead1.type << ", is_end: " << dead1.is_red << std::endl;
    }*/
}

void test8() {
    ChessBoard board;
    board.ClearBoard();
    /*board.SetChessAt(Chess(ChessType::Wang, false), 7, 4);
    board.SetChessAt(Chess(ChessType::Shi, true), 7,5);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 1);
    board.SetChessAt(Chess(ChessType::Shi, true), 1,4);
    board.SetChessAt(Chess(ChessType::Xiang, true), 4,2);
    board.SetChessAt(Chess(ChessType::Ma, true), 2,3);*/
    board.initBoard();
    //std::cout << board.ToString() << std::endl;
    board.ParseFromString(board.ToString());
    //board.PrintOnTerminal();
}


double CalculateStandardDeviation(const std::vector<int> &data) {
    double mean = 0.0;
    double squaredDifferencesSum = 0.0;
    size_t dataSize = data.size();

    // 计算平均值
    for (int value: data) {
        mean += value;
    }
    mean /= dataSize;

    // 计算差值的平方和
    for (int value: data) {
        double difference = value - mean;
        squaredDifferencesSum += difference * difference;
    }

    // 计算平方差值的平均值
    double meanSquaredDifferences = squaredDifferencesSum / dataSize;

    // 计算标准差
    double standardDeviation = std::sqrt(meanSquaredDifferences);

    return standardDeviation;
}

void test10() {
    auto start = std::chrono::steady_clock::now();
    ChessBoard board;
    board.ClearBoard();
    //board.initBoard();
    board.SetChessAt(Chess(ChessType::Ma, false), 3, 0);
    board.SetChessAt(Chess(ChessType::Ma, false), 3, 2);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 4);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 6);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 8);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 0);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 2);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 4);
    board.SetChessAt(Chess(ChessType::Ma, true), 6, 6);
    board.SetChessAt(Chess(ChessType::Ma, true), 6, 8);
    ChessMove move(0, 0, 0, 0);
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, int> frequency;
    int n = 10;
    bool is_red_now = true;
    while (n--) {
        move = board.RandMove2(is_red_now);
        is_red_now = !is_red_now;
        ++frequency[std::make_pair(std::make_pair(move.end_x, move.end_y), std::make_pair(move.start_x, move.start_y))];
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 输出执行时间
    std::cout << "程序执行时间: " << duration << " 毫秒" << std::endl;
    for (const auto &entry: frequency) {
        int end_x = entry.first.first.first;
        int end_y = entry.first.first.second;
        int start_x = entry.first.second.first;
        int start_y = entry.first.second.second;
        std::cout << "Move (" << end_x << ", " << end_y << ", " << start_x << ", " << start_y
                  << ") appears " << entry.second << " times." << std::endl;
    }

    std::vector<int> values;
    for (const auto &entry: frequency) {
        values.push_back(entry.second);
    }

    double standardDeviation = CalculateStandardDeviation(values);
    std::cout << "Standard deviation of values: " << standardDeviation << std::endl;
    // 计算执行时间
    std::vector<Chess> dead;
    board.Move(ChessMove(7, 1, 0, 1));
}

void test11() {
    ChessBoard board;
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Shi, false), 0, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 8, 3);
    board.SetChessAt(Chess(ChessType::Shi, true), 7, 3);
    std::vector<ChessMove> moves;
    board.GetMovesFrom(8, 3, &moves);
    assert(moves.size() == 2);
}


void test12() {
    ChessBoard board;
    board.ClearBoard();
    board.initBoard();
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    int score;
    score = board.EvaluatePosition();
    std::cout << score << std::endl;
}

void test13() {
    std::string temp = R"([{"col":2,"is_red":false,"row":0,"type":6},{"col":5,"is_red":false,"row":0,"type":3},{"col":3,"is_red":false,"row":2,"type":0},{"col":5,"is_red":false,"row":2,"type":3},{"col":1,"is_red":false,"row":7,"type":2},{"col":3,"is_red":true,"row":8,"type":0},{"col":5,"is_red":false,"row":8,"type":2},{"col":8,"is_red":false,"row":9,"type":2}])";
    ChessBoard board;
    board.ParseFromString(temp);
    std::vector<ChessMove> moves;
    board.GetMoves(true, &moves);
    assert(!moves.empty());
}

void test14() {
    ChessBoard board;
    board.initBoard();
    board.Reverse();
    ChessBoard board1;
    board1.ParseFromString(board.ToString());
    assert(board.Move(ChessMove(6, 4, 5, 4)));
    assert(board1.Move(ChessMove(6, 4, 5, 4)));

}

void test15() {
    ChessBoard board;
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 4);
    std::vector<ChessMove> moves;
    board.GetMovesFrom(9, 4, &moves);
    assert(std::find(moves.begin(), moves.end(), ChessMove(9, 4, 0, 4)) != moves.end());
    assert(moves.size() == 4);
    board.SetChessAt(Chess(ChessType::Pao, true), 4, 4);
    moves.clear();
    board.GetMoves(false, &moves);
    assert(moves.size() == 3);

}

void test16() {
    ChessBoard board;
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Bing, true), 1, 3);
    board.SetChessAt(Chess(ChessType::Bing, true), 1, 5);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
    board.SetChessAt(Chess(ChessType::Bing, true), 3, 4);
    std::string QiPu;
    std::vector<ChessMove> moves;
    board.MoveConversion(ChessMove(3, 4, 2, 4), &QiPu);
    board.Move(ChessMove(3, 4, 2, 4));
    assert(QiPu == "兵五进一");
    board.MoveConversion(ChessMove(2, 5, 1, 4), &QiPu);
    board.Move(ChessMove(2, 5, 1, 4));
    assert(QiPu == "仕6退5");
    board.MoveConversion(ChessMove(1, 5, 1, 4), &QiPu);
    board.Move(ChessMove(1, 5, 1, 4));
    assert(QiPu == "兵四平五");
    board.MoveConversion(ChessMove(2, 3, 1, 4), &QiPu);
    board.Move(ChessMove(2, 3, 1, 4));
    assert(QiPu == "仕4退5");
    board.MoveConversion(ChessMove(2, 4, 1, 4), &QiPu);
    board.Move(ChessMove(2, 4, 1, 4));
    std::cout << QiPu << std::endl;
    assert(QiPu == "兵五进一");
}

void test17() {
    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Pao, true), 5, 4);
    board.SetChessAt(Chess(ChessType::Pao, false), 5, 7);
    board.SetChessAt(Chess(ChessType::Ma, false), 5, 9);
    board.SetChessAt(Chess(ChessType::Bing, true), 2, 4);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 4);
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 4, &moves);
        moves.clear();
    }
    board.Move(ChessMove(5, 4, 4, 4));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 4, &moves);
        moves.clear();
    }
    board.Move(ChessMove(4, 4, 4, 5));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 4, &moves);
        moves.clear();
    }
    std::cout << "Pao simulation " << n*3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}

void test18() {
    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Ju, true), 5, 4);
    board.SetChessAt(Chess(ChessType::Pao, false), 5, 7);
    board.SetChessAt(Chess(ChessType::Ma, false), 5, 9);
    board.SetChessAt(Chess(ChessType::Bing, true), 2, 4);
    board.SetChessAt(Chess(ChessType::Bing, false), 3, 4);
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 4, &moves);
        moves.clear();
    }
    board.Move(ChessMove(5, 4, 4, 4));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 4, &moves);
        moves.clear();
    }
    board.Move(ChessMove(4, 4, 4, 5));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 4, &moves);
        moves.clear();
    }
    std::cout << "Ju simulation " << n*3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}

void test19() {
    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Ju, true), 3, 6);
    board.SetChessAt(Chess(ChessType::Pao, false), 5, 4);
    board.SetChessAt(Chess(ChessType::Ma, false), 5, 5);
    board.SetChessAt(Chess(ChessType::Bing, true), 5, 6);
    board.SetChessAt(Chess(ChessType::Bing, false), 7, 6);
    board.SetChessAt(Chess(ChessType::Ma, false), 5, 0);
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 5, &moves);
        moves.clear();
    }
    board.Move(ChessMove(5, 4, 4, 4));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 5, &moves);
        moves.clear();
    }
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 0, &moves);
        moves.clear();
    }
    std::cout << "Ma simulation " << n*3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}
void test20() {
    ChessBoard board;
    board.initBoard();
    board.Move(ChessMove(9, 2, 7, 4));
    board.Move(ChessMove(6, 2, 5, 2));
    board.Move(ChessMove(9, 6, 7, 8));
    board.Move(ChessMove(3, 6, 4, 6));
    board.Move(ChessMove(4, 6, 5, 6));
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(7, 4, &moves);
        moves.clear();
    }
    board.Move(ChessMove(9, 1, 8, 3));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(7, 4, &moves);
        moves.clear();
    }
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(7, 8, &moves);
        moves.clear();
    }
    std::cout << "Xiang simulation " << n * 3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}
void test21() {
    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Shi, false), 0, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 3);
    board.SetChessAt(Chess(ChessType::Shi, true), 8, 4);
    board.SetChessAt(Chess(ChessType::Bing, false), 7, 3);
    board.SetChessAt(Chess(ChessType::Bing, true), 1, 4);
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(0, 3, &moves);
        moves.clear();
    }
    board.Move(ChessMove(2, 5, 1, 4));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(1, 4, &moves);
        moves.clear();
    }
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(8, 5, &moves);
        moves.clear();
    }
    std::cout << "Shi simulation " << n * 3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}
void test22() {
    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Shi, false), 0, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 3);
    board.SetChessAt(Chess(ChessType::Shi, true), 8, 4);
    board.SetChessAt(Chess(ChessType::Ma, false), 1, 3);
    board.SetChessAt(Chess(ChessType::Bing, true), 1, 4);
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(0, 4, &moves);
        moves.clear();
    }
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(9, 3, &moves);
        moves.clear();
    }
    board.Move(ChessMove(0, 4, 1, 4));
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(1, 4, &moves);
        moves.clear();
    }
    std::cout << "Wang simulation " << n * 3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;

}
void test23() {
    ChessBoard board;
    board.SetChessAt(Chess(ChessType::Bing, false), 5, 6);
    board.SetChessAt(Chess(ChessType::Bing, true), 6, 2);
    board.SetChessAt(Chess(ChessType::Bing, false), 9, 4);
    auto start = common::TimeUtility::CLockRealTimeMs();
    int n = 10000000;
    std::vector<ChessMove> moves;
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(5, 6, &moves);
        moves.clear();
    }
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(6, 2, &moves);
        moves.clear();
    }
    for (int i = 0; i < n; i++) {
        board.GetMovesFrom(9, 4, &moves);
        moves.clear();
    }
    std::cout << "Bing simulation " << n * 3 << " cost time:" << common::TimeUtility::CLockRealTimeMs() - start << std::endl;
}
void test24() {
    ChessBoard::Hash hash;
    ChessBoard board;
    board.ParseFromString(
            R"([{"type": 2, "is_red": true, "row": 0, "col": 3}, {"type": 0, "is_red": false, "row": 0, "col": 5}, {"type": 2, "is_red": true, "row": 1, "col": 4}, {"type": 2, "is_red": true, "row": 2, "col": 5}, {"type": 4, "is_red": false, "row": 8, "col": 0}, {"type": 0, "is_red": true, "row": 9, "col": 4}])");
    auto hash1=hash(board);
    ChessMove move(8,0,9,0);
    auto hash2=ChessBoard::Hash::getNextHash(hash1,board,move);
    assert(board.Move(move));
    auto hash3=hash(board);
    assert(hash3==hash2);

}
void test25(){
    ChessBoard board;
    board.ParseFromString(R"([{"col":3,"is_red":true,"row":0,"type":2},{"col":5,"is_red":false,"row":0,"type":0},{"col":4,"is_red":true,"row":1,"type":2},{"col":5,"is_red":true,"row":2,"type":2},{"col":0,"is_red":false,"row":8,"type":4},{"col":4,"is_red":true,"row":9,"type":0}])");
    std::string move_string;
    board.MoveConversion(ChessMove(8,0,8,2),&move_string);
    assert(move_string=="车1平3");
}

int main(int argc, char *argv[]) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test10();
    test11();
    test12();
    test13();
    test14();
    test15();
    test16();
    test17();
    test18();
    test19();
    test20();
    test21();
    test22();
    test23();
    test24();
    test25();
}
