//
// Created by zrr on 24-3-24.
//
#include "ChessBoard.h"
#include <vector>
#include  <assert.h>
#include <algorithm>
#include <iostream>

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
    std::vector<Chess> dead;
    std::vector<std::string> QiPu;
    /*board.GetMovesFrom(2, 0, &moves);
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 0, 1)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 4, 1)) != moves.end());
    board.Move(ChessMove(9, 5, 8 , 5),&dead);*/
    board.ChessConversion(ChessMove(7, 1, 7 , 4), &QiPu);
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
    std::vector<std::string> QiPu;
    board.GetMovesFrom(7, 1, &moves);
    board.ChessConversion(ChessMove(7, 1, 0, 1), &QiPu);
    board.Move(ChessMove(7, 1, 0, 1),&dead);
    assert(QiPu.size() == 1);
    assert(std::find(QiPu.begin(),QiPu.end(),"炮八进七") != QiPu.end());
    assert(dead.size() == 1);

}

void test7() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(false);
    board.SetChessAt(Chess(ChessType::Wang, false), 7, 4);
    board.SetChessAt(Chess(ChessType::Shi, true), 7,5);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 1);
    board.SetChessAt(Chess(ChessType::Shi, true), 1,4);
    board.SetChessAt(Chess(ChessType::Xiang, true), 4,2);
    board.SetChessAt(Chess(ChessType::Ma, true), 2,3);
    //board.SetChessAt(Chess(ChessType::Ma, true), 6,3);
    assert(board.Legal() == "Legal");
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

void test9() {

}
int main(int argc, char *argv[]) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    //test8();
}
