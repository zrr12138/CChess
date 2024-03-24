//
// Created by zhengran on 2024/3/18.
//

#ifndef CCHESS_CHESSBOARD_H
#define CCHESS_CHESSBOARD_H

#include <vector>
#include <string>

namespace CChess {
    enum ChessType {
        Empty = -1,
        Wang = 0,
        Ma = 1,
        Bing = 2,
        Shi = 3,
        Ju = 4,
        Pao = 5,
        Xiang = 6,
    };
    struct ChessMove {
        int start_x;
        int start_y;
        int end_x;
        int end_y;

        ChessMove(int startX, int startY, int endX, int endY);

        bool operator==(const ChessMove &rhs) const;

        bool operator!=(const ChessMove &rhs) const;
    };
    enum BoardResult {
        RED_WIN = 0,
        BLACK_WIN = 1,
        NOT_END = 2,
    };
    struct Chess {
        ChessType type;
        bool is_red;

        Chess(ChessType type, bool isRed);
    };

    class ChessBoard {
    private:
        Chess board[10][9];
    public:
        ChessBoard();

        void GetMoves(bool is_red, std::vector<ChessMove> *moves) const;

        void GetMovesFrom(int x, int y, std::vector<ChessMove> *move) const;

        Chess GetChessAt(int x, int y) const;

        bool Move(const ChessMove &move);

        BoardResult End() const;

        void ClearBoard();

        void initBoard();

        bool SetChessAt(const Chess &chess, int x, int y);

        void ParseFromString(const std::string &str);

        std::string ToString() const;
    };
}


#endif //CCHESS_CHESSBOARD_H
