//
// Created by zhengran on 2024/3/18.
//

#ifndef CCHESS_CHESSBOARD_H
#define CCHESS_CHESSBOARD_H
#include <vector>
namespace CChess {
    enum ChessType{
        Empty=-1,
        Wang=0,
        Ma=1,
        Bing=2,
        Shi=3,
        Ju=4,
        Pao=5,
        Xiang=6,
    };
    struct ChessMove{
        int start_x;
        int start_y;
        int end_x;
        int end_y;
    };
    enum BoardResult {
        RED_WIN = 0,
        BLACK_WIN = 1,
        NOT_END = 2,
    };
    class ChessBoard {
    private:
        ChessType board[10][9];
    public:
        void GetMoves(bool is_black, std::vector<ChessMove> *moves) const;
        ChessType GetChessAt(int x, int y) const;
        bool Move(ChessMove move);
        BoardResult End() const;
        void ClearBoard();
        void initBoard();
    };
}


#endif //CCHESS_CHESSBOARD_H
