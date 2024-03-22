//
// Created by zhengran on 2024/3/18.
//

#include "ChessBoard.h"
namespace CChess {


    void ChessBoard::GetMoves(bool is_black, std::vector<ChessMove> *moves) const {

    }

    ChessType ChessBoard::GetChessAt(int x, int y) const {
        return Ma;
    }

    bool ChessBoard::Move(ChessMove move) {
        return false;
    }

    BoardResult ChessBoard::End() const {
        return NOT_END;
    }

    void ChessBoard::ClearBoard() {

    }

    void ChessBoard::initBoard() {

    }
}