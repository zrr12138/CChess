//
// Created by zhengran on 2024/3/18.
//

#include "ChessBoard.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include <chrono>

namespace CChess {
    inline bool isWithinBounds(int x, int y) {
        return (x >= 0 && x < 10 && y >= 0 && y < 9);
    }

    void
    ChessBoard::AddMoveIfValid(int start_x, int start_y, int end_x, int end_y, std::vector<ChessMove> *moves) const {
        if (start_x != end_x || start_y != end_y) {
            moves->emplace_back(start_x, start_y, end_x, end_y);
        }
    }

    void ChessBoard::JuRule(int row, int col, std::vector<ChessMove> *moves) const {
//    if (!board[row][col].is_red) {
        // 检查车的水平向左运动
        for (int i = col - 1; i >= 0; i--) {
            if (board[row][i].IsEmpty()) {
                AddMoveIfValid(row, col, row, i, moves);
            } else {
                if (board[row][col].is_red != board[row][i].is_red) {
                    AddMoveIfValid(row, col, row, i, moves);
                }
                break;
            }
        }

        // 检查车的水平向右移动
        for (int i = col + 1; i < 9; i++) {
            if (board[row][i].IsEmpty()) {
                AddMoveIfValid(row, col, row, i, moves);
            } else {
                if (board[row][col].is_red != board[row][i].is_red) {
                    AddMoveIfValid(row, col, row, i, moves);
                }
                break;
            }
        }

        // 检查车的垂直移动向上
        for (int i = row - 1; i >= 0; i--) {
            if (board[i][col].IsEmpty()) {
                AddMoveIfValid(row, col, i, col, moves);
            } else {
                if (board[row][col].is_red != board[i][col].is_red) {
                    AddMoveIfValid(row, col, i, col, moves);
                }
                break;
            }
        }

        // 检查车的垂直向下移动
        for (int i = row + 1; i < 10; i++) {
            if (board[i][col].IsEmpty()) {
                AddMoveIfValid(row, col, i, col, moves);
            } else {
                if (board[row][col].is_red != board[i][col].is_red) {
                    AddMoveIfValid(row, col, i, col, moves);
                }
                break;
            }
        }
//        }
    }

    void ChessBoard::PaoRule(int row, int col, std::vector<ChessMove> *moves) const {
        int dir[4][2] = {{1,  0},
                         {-1, 0},
                         {0,  1},
                         {0,  -1}};
        int max_steps[4] = {9 - row, row, 8 - col, col};
        for (int d = 0; d < 4; d++) {
            int dx = dir[d][0], dy = dir[d][1];
            int x = row, y = col;
            bool cross = false;
            while (max_steps[d]--) {
                x += dx;
                y += dy;
                if (board[x][y].IsEmpty()) {
                    if (cross) continue;
                    else moves->emplace_back(row, col, x, y);
                } else {
                    if (cross) {
                        if (board[x][y].is_red != board[row][col].is_red) {
                            moves->emplace_back(row, col, x, y);
                        }
                        break;
                    } else {
                        cross = true;
                    }
                }
            }
        }
    }


    void ChessBoard::MaRule(int row, int col, std::vector<ChessMove> *moves) const {
        // 八个可能的马脚位置
        int offsets[8][2] = {{-2, -1},
                             {-2, 1},
                             {-1, 2},
                             {1,  2},
                             {2,  1},
                             {2,  -1},
                             {1,  -2},
                             {-1, -2}};
        // 四个阻挡的位置
        int offsets2[4][2] = {{-1, 0},
                              {0,  1},
                              {1,  0},
                              {0,  -1}};
        for (int i = 0; i < 8; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            int mid_x = row + offsets2[i / 2][0];
            int mid_y = col + offsets2[i / 2][1];
            if (0 <= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                if (board[mid_x][mid_y].IsEmpty()) {
                    if (board[end_x][end_y].IsEmpty())
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                    else {
                        if (board[end_x][end_y].is_red != board[row][col].is_red)
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                    }
                }
            }
        }
    }

    void ChessBoard::XiangRule(int row, int col, std::vector<ChessMove> *moves) const {
        // 四个可能的象眼的位置
        int offsets[4][2] = {{-2, -2},
                             {-2, 2},
                             {2,  -2},
                             {2,  2}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            int mid_x = (row + end_x) / 2;
            int mid_y = (col + end_y) / 2;
            if (0 <= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                if ((row < 5 && end_x < 5) || (row >= 5 && end_x >= 5)) {
                    if (board[mid_x][mid_y].IsEmpty()) {
                        if (board[end_x][end_y].IsEmpty())
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                        else {
                            if (board[end_x][end_y].is_red != board[row][col].is_red)
                                AddMoveIfValid(row, col, end_x, end_y, moves);
                        }
                    }
                }
            }
        }
    }

    void ChessBoard::ShiRule(int row, int col, std::vector<ChessMove> *moves) const {
        // 五个可能的士的位置
        int offsets[4][2] = {{-1, -1},
                             {-1, 1},
                             {1,  -1},
                             {1,  1}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            if (3 <= end_y && end_y <= 5) {
                if ((end_x < 3 && end_x >= 0) || (end_x > 6 && end_x <= 9)) {
                    if (board[end_x][end_y].IsEmpty()) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                    } else {
                        if (board[row][col].is_red != board[end_x][end_y].is_red) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                        }
                    }
                }
            }
        }
    }

    void ChessBoard::WangRule(int row, int col, std::vector<ChessMove> *moves) const {
        // 四个可能的帅的移动位置
        int offsets[4][2] = {{-1, 0},
                             {0,  1},
                             {1,  0},
                             {0,  -1}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            if (3 <= end_y && end_y <= 5) {
                if (end_x < 3 && end_x >= 0) {
                    if (board[end_x][end_y].IsEmpty()) {
                        for (int j = row + 1; j < 10; j++) {
                            if (board[j][end_y].type != Wang && board[j][end_y].type != Empty) {
                                AddMoveIfValid(row, col, end_x, end_y, moves);
                            }
                        }
                    } else {
                        if (board[row][col].is_red != board[end_x][end_y].is_red) {
                            for (int j = row + 1; j < 10; j++) {
                                if (board[j][end_y].type != Wang && board[j][end_y].type != Empty) {
                                    AddMoveIfValid(row, col, end_x, end_y, moves);
                                }
                            }
                        }
                    }
                } else if (end_x > 6 && end_x <= 9) {
                    if (board[end_x][end_y].IsEmpty()) {
                        for (int j = row - 1; j >= 0; j--) {
                            if (board[j][end_y].type != Wang && board[j][end_y].type != Empty) {
                                AddMoveIfValid(row, col, end_x, end_y, moves);
                            }
                        }
                    } else {
                        if (board[row][col].is_red != board[end_x][end_y].is_red) {
                            for (int j = row - 1; j >= 0; j--) {
                                if (board[j][end_y].type != Wang && board[j][end_y].type != Empty) {
                                    AddMoveIfValid(row, col, end_x, end_y, moves);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void ChessBoard::BingRule(int row, int col, std::vector<ChessMove> *moves) const {
        //兵的移动位置 判断是否过河;
        int ReGuohe[3][2] = {{-1, 0},
                             {0,  1},
                             {0,  -1}};
        int BaGuohe[3][2] = {{0, -1},
                             {0, 1},
                             {1, 0}};
        for (int i = 0; i < 3; i++) {
            int end_x;
            int end_y;
            if ((board[row][col].is_red && board_red) || (!board[row][col].is_red && !board_red)) {
                if (row < 5) {
                    end_x = row + ReGuohe[i][0];
                    end_y = col + ReGuohe[i][1];
                    if (0 <= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                        if (board[end_x][end_y].IsEmpty()) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                        } else {
                            if (board[row][col].is_red != board[end_x][end_y].is_red) {
                                AddMoveIfValid(row, col, end_x, end_y, moves);
                            }
                        }
                    }
                } else {
                    end_x = row - 1;
                    end_y = col;
                    if (board[end_x][end_y].IsEmpty()) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                        break;
                    } else {
                        if (board[row][col].is_red != board[end_x][end_y].is_red) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                            break;
                        }
                    }
                }
            } else if ((!board[row][col].is_red && board_red) || (board[row][col].is_red && !board_red)) {
                if (row > 4) {
                    end_x = row + BaGuohe[i][0];
                    end_y = col + BaGuohe[i][1];
                    if (0 <= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                        if (board[end_x][end_y].IsEmpty()) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                        } else {
                            if (board[row][col].is_red != board[end_x][end_y].is_red) {
                                AddMoveIfValid(row, col, end_x, end_y, moves);
                            }
                        }
                    }
                } else {
                    end_x = row + 1;
                    end_y = col;
                    if (board[end_x][end_y].IsEmpty()) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                        break;
                    } else {
                        if (board[row][col].is_red != board[end_x][end_y].is_red) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                            break;
                        }
                    }
                }
            }
        }
    }


    void ChessBoard::GetMoves(bool is_red, std::vector<ChessMove> *moves) const {
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 9; col++) {
                if (!board[row][col].IsEmpty()) {
                    if (is_red == board[row][col].is_red) {
                        switch (board[row][col].type) {
                            case Ju:
                                JuRule(row, col, moves);
                                break;
                            case Pao:
                                PaoRule(row, col, moves);
                                break;
                            case Ma:
                                MaRule(row, col, moves);
                                break;
                            case Xiang:
                                XiangRule(row, col, moves);
                                break;
                            case Shi:
                                ShiRule(row, col, moves);
                                break;
                            case Wang:
                                WangRule(row, col, moves);
                                break;
                            case Bing:
                                BingRule(row, col, moves);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }

    Chess ChessBoard::GetChessAt(int x, int y) const {
        assert(0 <= x && x < 10);
        assert(0 <= x && x < 9);
        return {board[x][y].type, board[x][y].is_red};
    }

    bool ChessBoard::Move(const ChessMove &move) {
        assert(0 <= move.end_x && move.end_x < 10);
        assert(0 <= move.end_x && move.end_y < 9);
        auto &end_chess = board[move.end_x][move.end_y];
        auto &start_chess = board[move.start_x][move.start_y];
        if (start_chess.IsEmpty()) {
            return false;
        }
        if (!end_chess.IsEmpty() && start_chess.is_red == end_chess.is_red) {
            return false;
        }
        std::vector<ChessMove> moves;
        GetMovesFrom(move.start_x, move.start_y, &moves);
        if (std::find(moves.begin(), moves.end(), move) == moves.end()) {
            return false;
        }
        if (!end_chess.IsEmpty() && end_chess.type == ChessType::Wang) {
            end = end_chess.is_red ? BoardResult::BLACK_WIN : BoardResult::RED_WIN;
        }
        end_chess = start_chess;
        start_chess.SetEmpty();
        return true;
    }

    BoardResult ChessBoard::End() const {
        switch (end) {
            case 2:
                return BoardResult::NOT_END;
            case 1:
                return BoardResult::BLACK_WIN;
            case 0:
                return BoardResult::RED_WIN;
        }
    }

    void ChessBoard::ClearBoard() {
        end = NOT_END;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                board[i][j].SetEmpty();
            }
        }
    }

    void ChessBoard::initBoard() {
        ClearBoard();
        if (board_red) {
            SetChessAt(Chess(ChessType::Ju, true), 9, 0);
            SetChessAt(Chess(ChessType::Ma, true), 9, 1);
            SetChessAt(Chess(ChessType::Xiang, true), 9, 2);
            SetChessAt(Chess(ChessType::Shi, true), 9, 3);
            SetChessAt(Chess(ChessType::Wang, true), 9, 4);
            SetChessAt(Chess(ChessType::Shi, true), 9, 5);
            SetChessAt(Chess(ChessType::Xiang, true), 9, 6);
            SetChessAt(Chess(ChessType::Ma, true), 9, 7);
            SetChessAt(Chess(ChessType::Ju, true), 9, 8);
            SetChessAt(Chess(ChessType::Pao, true), 7, 1);
            SetChessAt(Chess(ChessType::Pao, true), 7, 7);
            SetChessAt(Chess(ChessType::Bing, true), 6, 0);
            SetChessAt(Chess(ChessType::Bing, true), 6, 2);
            SetChessAt(Chess(ChessType::Bing, true), 6, 4);
            SetChessAt(Chess(ChessType::Bing, true), 6, 6);
            SetChessAt(Chess(ChessType::Bing, true), 6, 8);

            SetChessAt(Chess(ChessType::Ju, false), 0, 0);
            SetChessAt(Chess(ChessType::Ma, false), 0, 1);
            SetChessAt(Chess(ChessType::Xiang, false), 0, 2);
            SetChessAt(Chess(ChessType::Shi, false), 0, 3);
            SetChessAt(Chess(ChessType::Wang, false), 0, 4);
            SetChessAt(Chess(ChessType::Shi, false), 0, 5);
            SetChessAt(Chess(ChessType::Xiang, false), 0, 6);
            SetChessAt(Chess(ChessType::Ma, false), 0, 7);
            SetChessAt(Chess(ChessType::Ju, false), 0, 8);
            SetChessAt(Chess(ChessType::Pao, false), 2, 1);
            SetChessAt(Chess(ChessType::Pao, false), 2, 7);
            SetChessAt(Chess(ChessType::Bing, false), 3, 0);
            SetChessAt(Chess(ChessType::Bing, false), 3, 2);
            SetChessAt(Chess(ChessType::Bing, false), 3, 4);
            SetChessAt(Chess(ChessType::Bing, false), 3, 6);
            SetChessAt(Chess(ChessType::Bing, false), 3, 8);
        } else {
            SetChessAt(Chess(ChessType::Ju, false), 9, 0);
            SetChessAt(Chess(ChessType::Ma, false), 9, 1);
            SetChessAt(Chess(ChessType::Xiang, false), 9, 2);
            SetChessAt(Chess(ChessType::Shi, false), 9, 3);
            SetChessAt(Chess(ChessType::Wang, false), 9, 4);
            SetChessAt(Chess(ChessType::Shi, false), 9, 5);
            SetChessAt(Chess(ChessType::Xiang, false), 9, 6);
            SetChessAt(Chess(ChessType::Ma, false), 9, 7);
            SetChessAt(Chess(ChessType::Ju, false), 9, 8);
            SetChessAt(Chess(ChessType::Pao, false), 7, 1);
            SetChessAt(Chess(ChessType::Pao, false), 7, 7);
            SetChessAt(Chess(ChessType::Bing, false), 6, 0);
            SetChessAt(Chess(ChessType::Bing, false), 6, 2);
            SetChessAt(Chess(ChessType::Bing, false), 6, 4);
            SetChessAt(Chess(ChessType::Bing, false), 6, 6);
            SetChessAt(Chess(ChessType::Bing, false), 6, 8);

            SetChessAt(Chess(ChessType::Ju, true), 0, 0);
            SetChessAt(Chess(ChessType::Ma, true), 0, 1);
            SetChessAt(Chess(ChessType::Xiang, true), 0, 2);
            SetChessAt(Chess(ChessType::Shi, true), 0, 3);
            SetChessAt(Chess(ChessType::Wang, true), 0, 4);
            SetChessAt(Chess(ChessType::Shi, true), 0, 5);
            SetChessAt(Chess(ChessType::Xiang, true), 0, 6);
            SetChessAt(Chess(ChessType::Ma, true), 0, 7);
            SetChessAt(Chess(ChessType::Ju, true), 0, 8);
            SetChessAt(Chess(ChessType::Pao, true), 2, 1);
            SetChessAt(Chess(ChessType::Pao, true), 2, 7);
            SetChessAt(Chess(ChessType::Bing, true), 3, 0);
            SetChessAt(Chess(ChessType::Bing, true), 3, 2);
            SetChessAt(Chess(ChessType::Bing, true), 3, 4);
            SetChessAt(Chess(ChessType::Bing, true), 3, 6);
            SetChessAt(Chess(ChessType::Bing, true), 3, 8);
        }
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j].type != Empty) {
                    Total.emplace_back(board[i][j].type, board[i][j].is_red);
                }
            }
        }
    }


    void ChessBoard::GetMovesFrom(int x, int y, std::vector<ChessMove> *move) const {
        GetMoves(board[x][y].is_red, move);
        auto it = move->begin();
        while (it != move->end()) {
            int start_x = it->start_x;
            int start_y = it->start_y;
            if (start_x != x || start_y != y) {
                it = move->erase(it);
            } else {
                it++;
            }
        }
    }

    void ChessBoard::SetChessAt(const Chess &chess, int x, int y) {
        assert(!chess.IsEmpty());
        board[x][y] = chess;
    }

    void ChessBoard::ParseFromString(const std::string &str) {
        nlohmann::json jsonBoard = nlohmann::json::parse(str);
        ClearBoard();
        for (const auto &jsonChess: jsonBoard) {
            int row = jsonChess["row"];
            int col = jsonChess["col"];
            ChessType type = jsonChess["type"];
            bool isRed = jsonChess["is_red"];
            assert(type != Empty);
            assert(0 <= row && row < 10);
            assert(0 <= col && col < 10);
            Chess chess(type, isRed);
            SetChessAt(chess, row, col);
        }
    }

    std::string ChessBoard::ToString() const {
        nlohmann::json jsonBoard;

        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 9; col++) {
                const Chess &chess = board[row][col];
                if (chess.IsEmpty()) {
                    continue;
                }
                nlohmann::json jsonChess;
                jsonChess["type"] = chess.type;
                jsonChess["is_red"] = chess.is_red;
                jsonChess["row"] = row;
                jsonChess["col"] = col;

                jsonBoard.push_back(jsonChess);
            }
        }

        return jsonBoard.dump();
    }

    void ChessBoard::PrintOnTerminal() {
        for (int i = 0; i < 10; ++i) {
            std::cout << std::endl;
            for (int j = 0; j < 9; ++j) {
                char c;
                switch (board[i][j].type) {
                    case Ju:
                        c = '4';
                        break;
                    case Pao:
                        c = '5';
                        break;
                    case Ma:
                        c = '1';
                        break;
                    case Xiang:
                        c = '6';
                        break;
                    case Shi:
                        c = '3';
                        break;
                    case Wang:
                        c = '0';
                        break;
                    case Bing:
                        c = '2';
                        break;
                    default:
                        c = 'x';
                        break;
                }
                std::cout << c << " ";
            }
        }
        std::cout << std::endl;
    }

    ChessBoard::ChessBoard() {

    }

    void ChessBoard::PrintMoves(std::vector<ChessMove> *moves) const {
        for (const ChessMove &move: *moves) {
            std::cout << "StartX: " << move.start_x << ", StartY: " << move.start_y
                      << ", EndX: " << move.end_x << ", EndY: " << move.end_y << std::endl;
        }
    }

    void ChessBoard::BoardRed(bool is_red) {
        board_red = is_red;
    }


    bool ChessBoard::IsLegal(std::string *errorMessage) const {
        int num_qizi[7][2] = {0}; //0:Ju  1:Ma  2:Shi  3:Pao 4:Xiang 5:Wang 6:Bing
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                switch (board[i][j].type) {
                    case Ju:
                        if (board[i][j].is_red)
                            num_qizi[0][0]++;
                        else
                            num_qizi[0][1]++;
                        break;
                    case Wang:
                        if (board[i][j].is_red)
                            num_qizi[5][0]++;
                        else
                            num_qizi[5][1]++;
                        break;
                    case Ma:
                        if (board[i][j].is_red)
                            num_qizi[1][0]++;
                        else
                            num_qizi[1][1]++;
                        break;
                    case Bing:
                        if (board[i][j].is_red)
                            num_qizi[6][0]++;
                        else
                            num_qizi[6][1]++;
                        break;
                    case Shi:
                        if (board[i][j].is_red)
                            num_qizi[2][0]++;
                        else
                            num_qizi[2][1]++;
                        break;
                    case Pao:
                        if (board[i][j].is_red)
                            num_qizi[3][0]++;
                        else
                            num_qizi[3][1]++;
                        break;
                    case Xiang:
                        if (board[i][j].is_red)
                            num_qizi[4][0]++;
                        else
                            num_qizi[4][1]++;
                        break;
                    default:
                        break;
                }
            }
        }
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                switch (board[i][j].type) {
                    case Wang:
                        if (board_red) {
                            if (board[i][j].is_red) {
                                if ((3 <= j && j < 6) && (6 < i)) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "帅越界";
                                    return false;
                                }
                            } else {
                                if ((3 <= j && j < 6) && (0 <= i && i < 3)) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "将越界";
                                    return false;
                                }
                            }
                        } else {
                            if (!board[i][j].is_red) {
                                if ((3 <= j && j < 6) && (6 < i)) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "将越界";
                                    return false;
                                }
                            } else {
                                if ((3 <= j && j < 6) && (0 <= i && i < 3)) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "帅越界";
                                    return false;
                                }
                            }
                        }
                        break;
                    case Bing:
                        if (board_red) {
                            if (board[i][j].is_red) {
                                if (i == 5 || i == 6) {
                                    if (j == 0 || j == 2 || j == 4 || j == 6 || j == 8) {
                                        *errorMessage = "合法";
                                    } else {
                                        *errorMessage = "红兵越界";
                                        return false;
                                    }
                                } else if (0 <= i && i < 5) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "红兵越界";
                                    return false;
                                }
                            } else {
                                if (i == 3 || i == 4) {
                                    if (j == 0 || j == 2 || j == 4 || j == 6 || j == 8) {
                                        *errorMessage = "合法";
                                    } else {
                                        *errorMessage = "黑卒越界";
                                        return false;
                                    }
                                } else if (5 <= i) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "黑卒越届";
                                    return false;
                                }
                            }
                        } else {
                            if (!board[i][j].is_red) {
                                if (i == 5 || i == 6) {
                                    if (j == 0 || j == 2 || j == 4 || j == 6 || j == 8) {
                                        *errorMessage = "合法";
                                    } else {
                                        *errorMessage = "黑卒越界";
                                        return false;
                                    }
                                } else if (0 <= i && i < 5) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "黑卒越界";
                                    return false;
                                }
                            } else {
                                if (i == 3 || i == 4) {
                                    if (j == 0 || j == 2 || j == 4 || j == 6 || j == 8) {
                                        *errorMessage = "合法";
                                    } else {
                                        *errorMessage = "红兵越界";
                                        return false;
                                    }
                                } else if (5 <= i) {
                                    *errorMessage = "合法";
                                } else {
                                    *errorMessage = "红兵越界";
                                    return false;
                                }
                            }
                        }
                        break;
                    case Shi:
                        if (board_red) {
                            if (board[i][j].is_red) {
                                if (j == 3 || j == 5) {
                                    if (i == 0 || i == 2)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "仕越界";
                                        return false;
                                    }
                                } else if (j == 4) {
                                    if (i == 1)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "仕越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "仕越界";
                                    return false;
                                }
                            } else {
                                if (j == 3 || j == 5) {
                                    if (i == 7 || i == 9)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "士越界";
                                        return false;
                                    }
                                } else if (j == 4) {
                                    if (i == 8)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "士越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "士越界";
                                    return false;
                                }
                            }
                        } else {
                            if (board[i][j].is_red) {
                                if (j == 3 || j == 5) {
                                    if (i == 0 || i == 2)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "士越界";
                                        return false;
                                    }
                                } else if (j == 4) {
                                    if (i == 1)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "士越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "士越界";
                                    return false;
                                }
                            } else {
                                if (j == 3 || j == 5) {
                                    if (i == 7 || i == 9)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "仕越界";
                                        return false;
                                    }
                                } else if (j == 4) {
                                    if (i == 8)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "仕越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "仕越界";
                                    return false;
                                }
                            }
                        }
                        break;
                    case Xiang:
                        if (board_red) {
                            if (!board[i][j].is_red) {
                                if (j == 0 || j == 8 || j == 4) {
                                    if (i == 2)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "象越界";
                                        return false;
                                    }
                                } else if (j == 2 || j == 6) {
                                    if (i == 0 || i == 4)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "象越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "象越界";
                                    return false;
                                }
                            } else {
                                if (j == 0 || j == 8 || j == 4) {
                                    if (i == 7)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "相越界";
                                        return false;
                                    }
                                } else if (j == 2 || j == 6) {
                                    if (i == 5 || i == 9)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "相越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "相越界";
                                    return false;
                                }
                            }
                        } else {
                            if (board[i][j].is_red) {
                                if (j == 0 || j == 8 || j == 4) {
                                    if (i == 2)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "相越界";
                                        return false;
                                    }
                                } else if (j == 2 || j == 6) {
                                    if (i == 0 || i == 4)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "相越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "相越界";
                                    return false;
                                }
                            } else {
                                if (j == 0 || j == 8 || j == 4) {
                                    if (i == 7)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "象越界";
                                        return false;
                                    }
                                } else if (j == 2 || j == 6) {
                                    if (i == 5 || i == 9)
                                        *errorMessage = "合法";
                                    else {
                                        *errorMessage = "象越界";
                                        return false;
                                    }
                                } else {
                                    *errorMessage = "象越界";
                                    return false;
                                }
                            }
                        }

                        break;
                    default:
                        break;
                }
            }
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                if (num_qizi[i][j] > 2) {
                    if (j == 0) {
                        switch (i) {
                            case 0:
                                *errorMessage = "红车数量超过";
                                return false;
                            case 1:
                                *errorMessage = "红马数量超过";
                                return false;
                            case 2:
                                *errorMessage = "红士数量超过";
                                return false;
                            case 3:
                                *errorMessage = "红炮数量超过";
                                return false;
                            case 4:
                                *errorMessage = "红相数量超过";
                                return false;
                            default:
                                break;
                        }
                    } else {
                        switch (i) {
                            case 0:
                                *errorMessage = "黑车数量超过";
                                return false;
                            case 1:
                                *errorMessage = "黑马数量超过";
                                return false;
                            case 2:
                                *errorMessage = "黑士数量超过";
                                return false;
                            case 3:
                                *errorMessage = "黑炮数量超过";
                                return false;
                            case 4:
                                *errorMessage = "黑相数量超过";
                                return false;
                            default:
                                break;
                        }
                    }
                }
            }
        }
        if (num_qizi[5][0] > 1) {
            *errorMessage = "帅数量超过";
            return false;
        } else if (num_qizi[5][1] > 1) {
            *errorMessage = "黑将数量超过";
            return false;
        } else if (num_qizi[6][0] > 5) {
            *errorMessage = "红兵数量超过";
            return false;
        } else if (num_qizi[6][1] > 5) {
            *errorMessage = "黑卒数量超过";
            return false;
        }
        return true;
    }

    void ChessBoard::MoveConversion(const ChessMove &move, std::string *QiPu) {
        std::string Con;
        switch (board[move.start_x][move.start_y].type) {
            case Wang:
                if (board[move.start_x][move.start_y].is_red) {
                    Con = "帅";
                } else {
                    Con = "将";
                }
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            case Ma:
                Con = "马";
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            case Bing:
                if (board[move.start_x][move.start_y].is_red) {
                    Con = "兵";
                } else {
                    Con = "将卒";
                }
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            case Shi:
                if (board[move.start_x][move.start_y].is_red) {
                    Con = "士";
                } else {
                    Con = "仕";
                }
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            case Ju:
                Con = "车";
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            case Pao:
                Con = "炮";
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            case Xiang:
                if (board[move.start_x][move.start_y].is_red) {
                    Con = "相";
                } else {
                    Con = "象";
                }
                Con = Conversion(move, Con);
                *QiPu = Con;
                break;
            default:
                break;
        }
    }

    std::string ChessBoard::Conversion(const ChessMove &move, std::string conversion) {
        std::string Rename1;
        std::string Baname1;
        std::string Rename2;
        std::string Baname2;
        std::string Rename3;
        std::string Baname3;
        switch (move.start_y) {
            case 0:
                Rename1 = "九";
                Baname1 = "1";
                break;
            case 1:
                Rename1 = "八";
                Baname1 = "2";
                break;
            case 2:
                Rename1 = "七";
                Baname1 = "3";
                break;
            case 3:
                Rename1 = "六";
                Baname1 = "4";
                break;
            case 4:
                Rename1 = "五";
                Baname1 = "5";
                break;
            case 5:
                Rename1 = "四";
                Baname1 = "6";
                break;
            case 6:
                Rename1 = "三";
                Baname1 = "7";
                break;
            case 7:
                Rename1 = "二";
                Baname1 = "8";
                break;
            case 8:
                Rename1 = "一";
                Baname1 = "9";
                break;
            default:
                break;
        }
        switch (abs(move.start_x - move.end_x)) {
            case 9:
                Rename2 = "九";
                Baname2 = "9";
                break;
            case 8:
                Rename2 = "八";
                Baname2 = "8";
                break;
            case 7:
                Rename2 = "七";
                Baname2 = "7";
                break;
            case 6:
                Rename2 = "六";
                Baname2 = "6";
                break;
            case 5:
                Rename2 = "五";
                Baname2 = "5";
                break;
            case 4:
                Rename2 = "四";
                Baname2 = "4";
                break;
            case 3:
                Rename2 = "三";
                Baname2 = "3";
                break;
            case 2:
                Rename2 = "二";
                Baname2 = "2";
                break;
            case 1:
                Rename2 = "一";
                Baname2 = "1";
                break;
            default:
                break;
        }
        switch (move.end_y) {
            case 0:
                Rename3 = "九";
                Baname3 = "1";
                break;
            case 1:
                Rename3 = "八";
                Baname3 = "2";
                break;
            case 2:
                Rename3 = "七";
                Baname3 = "3";
                break;
            case 3:
                Rename3 = "六";
                Baname3 = "4";
                break;
            case 4:
                Rename3 = "五";
                Baname3 = "5";
                break;
            case 5:
                Rename3 = "四";
                Baname3 = "6";
                break;
            case 6:
                Rename3 = "三";
                Baname3 = "7";
                break;
            case 7:
                Rename3 = "二";
                Baname3 = "8";
                break;
            case 8:
                Rename3 = "一";
                Baname3 = "9";
                break;
            default:
                break;
        }
        if (board[move.start_x][move.start_y].type != Ma && board[move.start_x][move.start_y].type != Shi &&
            board[move.start_x][move.start_y].type != Xiang) {
            for (int i = 0; i < 10; i++) {
                if (i != move.start_x && board[i][move.start_y].is_red == board[move.start_x][move.start_y].is_red &&
                    board[i][move.start_y].type == board[move.start_x][move.start_y].type) {
                    if (board[move.start_x][move.start_y].is_red) {
                        if (i > move.start_x) {
                            if (move.start_x > move.end_x) {
                                conversion.insert(0, "前");
                                conversion += "进";
                                conversion += Rename2;
                            } else if (move.start_x < move.end_x) {
                                conversion.insert(0, "前");
                                conversion += "退";
                                conversion += Rename2;
                            } else {
                                conversion.insert(0, "前");
                                conversion += "平";
                                conversion += Rename3;
                            }
                        } else {
                            if (move.start_x > move.end_x) {
                                conversion.insert(0, "后");
                                conversion += "进";
                                conversion += Rename2;
                            } else if (move.start_x < move.end_x) {
                                conversion.insert(0, "后");
                                conversion += "退";
                                conversion += Rename2;
                            } else {
                                conversion.insert(0, "后");
                                conversion += "平";
                                conversion += Rename3;
                            }
                        }
                    } else {
                        if (i > move.start_x) {
                            if (move.start_x > move.end_x) {
                                conversion.insert(0, "前");
                                conversion += "退";
                                conversion += Baname2;
                            } else if (move.start_x < move.end_x) {
                                conversion.insert(0, "前");
                                conversion += "进";
                                conversion += Baname2;
                            } else {
                                conversion.insert(0, "前");
                                conversion += "平";
                                conversion += Baname3;
                            }
                        } else {
                            if (move.start_x > move.end_x) {
                                conversion.insert(0, "后");
                                conversion += "退";
                                conversion += Baname2;
                            } else if (move.start_x < move.end_x) {
                                conversion.insert(0, "后");
                                conversion += "进";
                                conversion += Baname2;
                            } else {
                                conversion.insert(0, "后");
                                conversion += "平";
                                conversion += Baname3;
                            }
                        }
                    }
                    break;
                } else {
                    if (board[move.start_x][move.start_y].is_red) {
                        if (move.start_x > move.end_x) {
                            conversion += Rename1;
                            conversion += "进";
                            conversion += Rename2;
                        } else if (move.start_x < move.end_x) {
                            conversion += Rename1;
                            conversion += "退";
                            conversion += Rename2;
                        } else {
                            conversion += Rename1;
                            conversion += "平";
                            conversion += Rename3;
                        }
                    } else {
                        if (move.start_x > move.end_x) {
                            conversion += Baname1;
                            conversion += "退";
                            conversion += Baname2;
                        } else if (move.start_x < move.end_x) {
                            conversion += Baname1;
                            conversion += "进";
                            conversion += Baname2;
                        } else {
                            conversion += Baname1;
                            conversion += "平";
                            conversion += Baname3;
                        }
                    }
                    break;
                }

            }
        } else {
            if (board[move.start_x][move.start_y].is_red) {
                if (move.start_x > move.end_x) {
                    conversion += Rename1;
                    conversion += "进";
                    conversion += Rename3;
                } else if (move.start_x < move.end_x) {
                    conversion += Rename1;
                    conversion += "退";
                    conversion += Rename3;
                }
            } else {
                if (move.start_x > move.end_x) {
                    conversion += Baname1;
                    conversion += "退";
                    conversion += Baname3;
                } else if (move.start_x < move.end_x) {
                    conversion += Baname1;
                    conversion += "进";
                    conversion += Baname3;
                }
            }
        }
        return conversion;
    }

    /*bool ChessBoard::RandMove(ChessMove &move) const {
        std::vector<ChessMove> moves;
        GetMoves(false, &moves);
        if (moves.empty()) {
            return false;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, moves.size() - 1);
        int index = dis(gen);
        move = moves[index];

        return true;
    }*/

    ChessMove ChessBoard::RandMove2(bool is_red) {
        assert(end == NOT_END);
        //ChessMove move(0, 0, 0, 0);
        std::vector<ChessMove> moves;
        GetMoves(is_red, &moves);
        std::random_device rd;
        //uint32_t seed = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, moves.size() - 1);
        std::size_t index = dist(gen);
        //move = moves[index];
        return moves[index];
    }

    uint32_t ChessBoard::Xorshift32() {
        uint32_t x = xorshift_state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        xorshift_state = x;
        return x;
    }

    int ChessBoard::EvaluatePosition() {
        int score = 0;
        int blackscore = 0;
        int redscore = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j].is_red) {
                    redscore += GetPieceValue(i, j, board[i][j].type);
                } else {
                    blackscore += GetPieceValue(i, j, board[i][j].type);
                }
            }
        }
        score = redscore - blackscore;
        return score;
    }

    int ChessBoard::GetPieceValue(int i, int j, ChessType chess) {
        switch (chess) {
            case Wang:
                return 100;
            case Ma:
                return 7;
            case Bing:
                if (board_red) {
                    if (board[i][j].is_red) {
                        if (i < 5)
                            return 4;
                        else
                            return 1;
                    } else {
                        if (i > 5)
                            return 4;
                        else
                            return 1;
                    }
                } else {
                    if (board[i][j].is_red) {
                        if (i > 5)
                            return 4;
                        else
                            return 1;
                    } else {
                        if (i < 5)
                            return 4;
                        else
                            return 1;
                    }
                }
            case Shi:
                return 3;
            case Ju:
                return 9;
            case Pao:
                return 6;
            case Xiang:
                return 3;
            default:
                return 0;
        }

    }

    void ChessBoard::GetDeadChess(std::vector<Chess> *dead) {
        dead->clear();
        std::vector<Chess> Live;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j].type != Empty) {
                    Live.emplace_back(board[i][j].type, board[i][j].is_red);
                }
            }
        }
        std::sort(Total.begin(), Total.end(), ChessComparator);
        std::sort(Live.begin(), Live.end(), ChessComparator);
        std::set_difference(Total.begin(), Total.end(), Live.begin(), Live.end(),
                            std::back_inserter(*dead), ChessComparator);
    }

    bool ChessBoard::ChessComparator(const Chess &chess1, const Chess &chess2) {
        if (chess1.type < chess2.type) {
            return true;
        } else if (chess1.type == chess2.type) {
            return chess1.is_red < chess2.is_red;
        } else {
            return false;
        }
    }

    Chess::Chess(ChessType type, bool isRed) : type(type), is_red(isRed) {}

    Chess::Chess() {
        SetEmpty();
    }

    bool Chess::IsEmpty() const {
        return type == Empty;
    }

    void Chess::SetEmpty() {
        type = Empty;
    }

    /*//ChessMove::ChessMove(int startX, int startY, int endX, int endY) : start_x(startX), start_y(startY), end_x(endX),
                                                                       end_y(endY) {}*/

    bool ChessMove::operator==(const ChessMove &rhs) const {
        return start_x == rhs.start_x &&
               start_y == rhs.start_y &&
               end_x == rhs.end_x &&
               end_y == rhs.end_y;
    }

    bool ChessMove::operator!=(const ChessMove &rhs) const {
        return !(rhs == *this);
    }

    ChessMove::ChessMove(int startX, int startY, int endX, int endY) : start_x(startX), start_y(startY), end_x(endX),
                                                                       end_y(endY) {}

    std::ostream &operator<<(std::ostream &os, const ChessMove &move) {
        os << "start_x: " << move.start_x << " start_y: " << move.start_y << " end_x: " << move.end_x << " end_y: "
           << move.end_y;
        return os;
    }

    std::string ChessMove::ToString() const {
        nlohmann::json jsonMove;
        jsonMove["start_x"] = start_x;
        jsonMove["start_y"] = start_y;
        jsonMove["end_x"] = end_x;
        jsonMove["end_y"] = end_y;
        return jsonMove.dump();
    }

    void ChessMove::ParseFromString(const std::string &str) {
        nlohmann::json jsonMove = nlohmann::json::parse(str);
        start_x = jsonMove["start_x"];
        start_y = jsonMove["start_y"];
        end_x = jsonMove["end_x"];
        end_y = jsonMove["end_y"];
    }

}