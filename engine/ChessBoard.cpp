//
// Created by zhengran on 2024/3/18.
//

#include "ChessBoard.h"
namespace CChess {


    void ChessBoard::GetMoves(bool red, std::vector<ChessMove> *moves) const {

    }

    ChessType ChessBoard::GetChessAt(int x, int y) const {
        return Ma;
    }

    bool ChessBoard::Move(const ChessMove &move) {
        return false;
    }

    BoardResult ChessBoard::End() const {
        return NOT_END;
    }

    void ChessBoard::ClearBoard() {

    }

    void ChessBoard::initBoard() {

    }

    Chess::Chess(ChessType type, bool isRed) : type(type), is_red(isRed) {}

    ChessMove::ChessMove(int startX, int startY, int endX, int endY) : start_x(startX), start_y(startY), end_x(endX),
                                                                       end_y(endY) {}

    bool ChessMove::operator==(const ChessMove &rhs) const {
        return start_x == rhs.start_x &&
               start_y == rhs.start_y &&
               end_x == rhs.end_x &&
               end_y == rhs.end_y;
    }

    bool ChessMove::operator!=(const ChessMove &rhs) const {
        return !(rhs == *this);
    }
}