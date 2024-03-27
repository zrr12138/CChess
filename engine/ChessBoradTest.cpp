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
    board.SetChessAt(Chess(ChessType::Ma, true), 2, 0);
    //board.SetChessAt(Chess(ChessType::Bing, false), 4, 0);
    board.SetChessAt(Chess(ChessType::Bing, true), 2, 1);
    board.SetChessAt(Chess(ChessType::Wang, true), 9, 5);
    board.SetChessAt(Chess(ChessType::Wang, false), 0, 4);
    //board.SetChessAt(Chess(ChessType::Pao, false), 0, 4);
    //board.SetChessAt(Chess(ChessType::Xiang, true), 5, 2);
    std::vector<ChessMove> moves;
    board.PrintOnTerminal();
    board.GetMoves(&moves);
    board.PrintMoves(&moves);
    //board.GetMovesFrom(2, 0, &moves);
    //assert(moves.size() == 2);
    //assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 0, 1)) != moves.end());
    //assert(std::find(moves.begin(), moves.end(), ChessMove(2, 0, 4, 1)) != moves.end());
}

int main(int argc, char *argv[]) {
    test1();
/*    ChessBoard board;
    board.ClearBoard();
    board.initBoard();
    board.PrintOnTerminal();*/

}
