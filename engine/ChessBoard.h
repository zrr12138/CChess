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

        Chess();

        bool IsEmpty() const;

        void SetEmpty();



        Chess(ChessType type, bool isRed);
    };

    class ChessBoard {

    private:
        Chess board[10][9];

        bool board_red;
        BoardResult is_end;
        bool is_init;
        int move_num;

        void update_is_end_from(ChessMove move); // 判断局面是否结束

        void AddMoveIfValid(int start_x, int start_y, int end_x, int end_y, std::vector<ChessMove>* moves) const;

        void JuRule(int row, int col, std::vector<ChessMove> *moves) const;

        void PaoRule(int row, int col, std::vector<ChessMove> *moves) const;

        void MaRule(int row, int col, std::vector<ChessMove> *moves) const;

        void XiangRule(int row, int col, std::vector<ChessMove> *moves) const;

        void ShiRule(int row, int col, std::vector<ChessMove> *moves) const;

        void WangRule(int row, int col, std::vector<ChessMove> *moves) const;

        void BingRule(int row, int col, std::vector<ChessMove> *moves) const;

        std::string Conversion(const ChessMove &move, std::string conversion);

    public:
        ChessBoard();

        void BoardRed(bool is_red);

        void GetMoves(bool is_red, std::vector<ChessMove> *moves) const;

        void GetMovesFrom(int x, int y, std::vector<ChessMove> *move) const; //位于xy的棋子走法

        Chess GetChessAt(int x, int y) const;

        bool Move(const ChessMove &move, std::vector<Chess> *dead);

        BoardResult End() const;

        void ClearBoard();

        void initBoard();

        void SetChessAt(const Chess &chess, int x, int y); //将chess放置到xy处

        void MoveConversion (const ChessMove &move, std::string *Con);

        void ParseFromString(const std::string &str); //json字符串

        std::string ToString() const;

        void PrintOnTerminal();

        void PrintMoves(std::vector<ChessMove> *moves) const;

        void GetDeadChess(std::vector<Chess> *deads) const;

        bool IsLegal(std::string *errorMessage) const;

        bool RandMove(ChessMove &move) const;

        bool RandMove2(ChessMove &move) const;
    };
}


#endif //CCHESS_CHESSBOARD_H
