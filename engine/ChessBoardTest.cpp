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
    board.BoardRed(true);
    board.initBoard();
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    std::string QiPu;
    /*board.GetMovesFrom(2, 0, &moves);
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 0, 1)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 4, 1)) != moves.end());
    */
    board.MoveConversion(ChessMove(7, 1, 1 , 0), &QiPu);
    board.GetDeadChess(ChessMove(7, 1, 0, 4), &dead);
    assert(dead.size() == 1);
}

void test2() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(true);
    board.SetChessAt(Chess(ChessType::Ju, false), 1, 6);
    board.SetChessAt(Chess(ChessType::Pao, true), 1, 8);
    board.SetChessAt(Chess(ChessType::Ju, false), 1, 4);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMovesFrom(1,8,&moves);
    assert(moves.size() ==  11);
    assert(std::find(moves.begin(), moves.end(), ChessMove(1, 8 ,1, 4)) != moves.end());
}

void test3() {
    ChessBoard board;
    board.BoardRed(true);
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Shi, true), 9, 5);
    //board.SetChessAt(Chess(ChessType::Ju, true), 8, 4);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMovesFrom(9,5,&moves);
    assert(moves.size() == 1);
    assert(std::find(moves.begin(), moves.end(), ChessMove(9, 5 ,8, 4)) != moves.end());
}

void test4() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(false);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 4);
    board.SetChessAt(Chess(ChessType::Bing, true), 6,0);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMovesFrom(6,0,&moves);
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), ChessMove(6, 0 ,6, 1)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(6, 0 ,7, 0)) != moves.end());
    board.GetMovesFrom(1,4,&moves);
    assert(moves.size() == 3);
}

void test5() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(false);
    board.SetChessAt(Chess(ChessType::Xiang, false), 7, 4);
    board.SetChessAt(Chess(ChessType::Xiang, true), 0,2);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 1);
    board.SetChessAt(Chess(ChessType::Bing, true), 8,3);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMovesFrom(7,4,&moves);
    assert(moves.size() == 3);
    assert(std::find(moves.begin(), moves.end(), ChessMove(7, 4 ,9, 6)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(7, 4 ,5, 6)) != moves.end());
}

void test6() {
    ChessBoard board;
    board.BoardRed(true);
    board.initBoard();
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    std::string QiPu;
    board.GetMovesFrom(7, 1, &moves);
    board.MoveConversion(ChessMove(7, 1, 0, 1), &QiPu);
    board.Move(ChessMove(7, 1, 0, 1));
    assert(QiPu == "炮八进七");

}

void test7() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(false);
    board.SetChessAt(Chess(ChessType::Wang, true), 2, 3);
    board.SetChessAt(Chess(ChessType::Shi, true), 2,5);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 1);
    board.SetChessAt(Chess(ChessType::Shi, true), 1,4);
    board.SetChessAt(Chess(ChessType::Xiang, true), 4,2);
    board.SetChessAt(Chess(ChessType::Ma, true), 2,3);
    //board.SetChessAt(Chess(ChessType::Ma, true), 6,3);
    std::string errorMessage;
    bool success = board.IsLegal(&errorMessage);
    //std::cout << errorMessage << std::endl;
    assert(board.IsLegal(&errorMessage) == true);

}

void test8() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(true);
    board.SetChessAt(Chess(ChessType::Wang, false), 7, 4);
    board.SetChessAt(Chess(ChessType::Shi, true), 7,5);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 1);
    board.SetChessAt(Chess(ChessType::Shi, true), 1,4);
    board.SetChessAt(Chess(ChessType::Xiang, true), 4,2);
    board.SetChessAt(Chess(ChessType::Ma, true), 2,3);
    std::cout << board.ToString() << std::endl;
    board.ParseFromString(board.ToString());
    board.PrintOnTerminal();
}


double CalculateStandardDeviation(const std::vector<int>& data) {
    double mean = 0.0;
    double squaredDifferencesSum = 0.0;
    size_t dataSize = data.size();

    // 计算平均值
    for (int value : data) {
        mean += value;
    }
    mean /= dataSize;

    // 计算差值的平方和
    for (int value : data) {
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
    board.BoardRed(true);
    board.initBoard();
    //std::vector<ChessMove> moves;
    //board.GetMoves(&moves, false);
    ChessMove move(0, 0, 0, 0);
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, int> frequency;
    int n = 10000000;
    while (n--) {
        move = board.RandMove2();
        ++frequency[std::make_pair(std::make_pair(move.end_x, move.end_y), std::make_pair(move.start_x, move.start_y))];
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 输出执行时间
    std::cout << "程序执行时间: " << duration << " 毫秒" << std::endl;
    for (const auto& entry : frequency) {
        int end_x = entry.first.first.first;
        int end_y = entry.first.first.second;
        int start_x = entry.first.second.first;
        int start_y = entry.first.second.second;
        std::cout << "Move (" << end_x << ", " << end_y << ", " << start_x << ", " << start_y
                  << ") appears " << entry.second << " times." << std::endl;
    }



    std::vector<int> values;
    for (const auto& entry : frequency) {
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
    board.BoardRed(true);
    board.SetChessAt(Chess(ChessType::Shi, false), 0, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 0, 5);
    board.SetChessAt(Chess(ChessType::Shi, false), 1, 4);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 3);
    board.SetChessAt(Chess(ChessType::Shi, false), 2, 5);
    //std::vector<ChessMove> moves;
    //board.GetMoves(&moves, false);
    ChessMove move(0, 0, 0, 0);
    std::map<std::pair<int, int>, int> frequency;
}


void  test12 () {
        ChessBoard board;
        board.ClearBoard();
        board.BoardRed(true);
        board.initBoard();
        std::vector<ChessMove> moves;
        std::vector<Chess> dead;
        int score;
        score = board.EvaluatePosition();
        std::cout << score << std::endl;
}
int main(int argc, char *argv[]) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test10();
    test11();
    //test12();
}
