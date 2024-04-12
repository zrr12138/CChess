//
// Created by zhengran on 2024/3/18.
//

#ifndef CCHESS_CHESSBOARD_H
#define CCHESS_CHESSBOARD_H

#include <vector>
#include <map>
#include <string>
#include <ostream>
#include <random>

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

        ChessMove()=default;

        ChessMove(int startX, int startY, int endX, int endY);

        bool operator==(const ChessMove &rhs) const;

        bool operator!=(const ChessMove &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const ChessMove &move);

        std::string ToString() const;

        void ParseFromString(const std::string &str);
    };
    enum BoardResult {
        RED_WIN = 0,
        BLACK_WIN = 1,
        NOT_END = 2,
    };

    std::string getBoardResultStr(BoardResult res);

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

        bool red_at_0{};

        BoardResult end;
        std::vector<Chess> Total;
        uint32_t xorshift_state{};


        void AddMoveIfValid(int start_x, int start_y, int end_x, int end_y, std::vector<ChessMove>* moves) const;

        void JuRule(int row, int col, std::vector<ChessMove> *moves) const;

        void PaoRule(int row, int col, std::vector<ChessMove> *moves) const;

        void MaRule(int row, int col, std::vector<ChessMove> *moves) const;

        void XiangRule(int row, int col, std::vector<ChessMove> *moves) const;

        void ShiRule(int row, int col, std::vector<ChessMove> *moves) const;

        void WangRule(int row, int col, std::vector<ChessMove> *moves) const;

        void BingRule(int row, int col, std::vector<ChessMove> *moves) const;

        std::string Conversion1(const ChessMove &move, std::string conversion);

        std::string Conversion2(const ChessMove &move, std::string conversion);

        std::string GetNumberName(int number, bool is_red);

        std::string GetFileRank(const ChessMove &move, bool isRed, std::string &Con);

        std::string GetColNumber(int col,bool is_red);

        uint32_t Xorshift32();

        int GetPieceValue(int x, int y, ChessType chess);

        static bool ChessComparator(const Chess& chess1, const Chess& chess2);

    public:

        explicit ChessBoard(bool red_at_0);

        ChessBoard();

        void GetMoves(bool is_red, std::vector<ChessMove> *moves) const;

        void GetMovesFrom(int x, int y, std::vector<ChessMove> *move) const; //位于xy的棋子走法

        const Chess & GetChessAt(int x, int y) const;

        bool Move(const ChessMove &move);

        BoardResult End() const;

        void ClearBoard();

        void initBoard();

        void SetChessAt(const Chess &chess, int x, int y); //将chess放置到xy处

        bool MoveConversion (const ChessMove &move, std::string *Con);

        void ParseFromString(const std::string &str); //json字符串

        std::string ToString() const;

        void PrintOnTerminal();

        void PrintMoves(std::vector<ChessMove> *moves) const;

        bool IsLegal(std::string *errorMessage) const;

        //bool RandMove(ChessMove &move) const;

        ChessMove RandMove2(bool is_red);

        int EvaluatePosition();

        void GetDeadChess (std::vector<Chess> *dead);

        void Reverse();

        struct Hash {
            std::size_t operator()(const ChessBoard& chessBoard) const;
        };

    };
}


#endif //CCHESS_CHESSBOARD_H
