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
    board.BoardRed(true);
    board.SetChessAt(Chess(ChessType::Ma, true), 2, 0);
    board.SetChessAt(Chess(ChessType::Xiang, false), 0, 1);
    board.SetChessAt(Chess(ChessType::Bing, true), 2, 1);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    board.SetChessAt(Chess(ChessType::Ju, true), 0, 8);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.PrintOnTerminal();
    //board.GetMoves(&moves);
    board.GetMovesFrom(2, 0, &moves);
    board.PrintMoves(&moves);
    //board.Move(ChessMove(2,0,0,1),&dead);
    //board.PrintOnTerminal();
    //board.Move(ChessMove(0, 8, 0, 4),&dead);
    //board.PrintOnTerminal();
    /*board.Move(ChessMove(0, 4, 4, 4),&dead);
    board.PrintOnTerminal();*/
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 0, 1)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 4, 1)) != moves.end());
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
    board.PrintOnTerminal();
    board.GetMovesFrom(1,8,&moves);
    board.PrintMoves(&moves);
}

void test3() {
    ChessBoard board;
    board.BoardRed(true);
    board.ClearBoard();
    board.SetChessAt(Chess(ChessType::Shi, true), 9, 5);
    //board.SetChessAt(Chess(ChessType::Ju, true), 8, 4);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.PrintOnTerminal();
    board.GetMovesFrom(9,5,&moves);
    board.PrintMoves(&moves);
}

void test4() {
    ChessBoard board;
    board.ClearBoard();
    board.BoardRed(false);
    board.SetChessAt(Chess(ChessType::Bing, false), 1, 4);
    board.SetChessAt(Chess(ChessType::Bing, true), 6,0);
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.PrintOnTerminal();
    board.GetMovesFrom(6,0,&moves);
    board.PrintMoves(&moves);
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
    board.PrintOnTerminal();
    board.GetMovesFrom(7,4,&moves);
    board.PrintMoves(&moves);
}

void test6() {
    ChessBoard board;
    board.initBoard();
    board.PrintOnTerminal();
    std::vector<ChessMove> moves;
    std::vector<Chess> dead;
    board.GetMovesFrom(7, 1, &moves);
    board.PrintMoves(&moves);
    board.Move(ChessMove(7, 1, 0, 1),&dead);
    board.PrintOnTerminal();
    board.PrintDead(&dead);
}

void test7() {
    int a;
    int b;
    a = 1;
    b = 2;
    int c = 4;
    assert(a + b == c);
}
int main(int argc, char *argv[]) {
    test1();
    //test2();
    //test3();
    //test4();
    //test6();
    //test7();
}
