//
// Created by zhengran on 2024/3/18.
//

#include "ChessBoard.h"
#include <cassert>
#include <iostream>

namespace CChess {

    void AddMoveIfValid(int start_x, int start_y, int end_x, int end_y, std::vector<ChessMove>* moves) {
        if (start_x != end_x || start_y != end_y) {
            moves->emplace_back(start_x, start_y, end_x, end_y);
        }
    }

    void JuRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
//    if (!board[row][col].is_red) {
            // 检查车的水平向左运动
            for (int i = col - 1; i >= 0; i--) {
                if (board[row][i].type == ChessType::Empty) {
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
                if (board[row][i].type == ChessType::Empty) {
                    AddMoveIfValid(row, col, row, i, moves);
                } else {
                    if (board[row][col].is_red != board[row][i].is_red) {
                        AddMoveIfValid(row, col, row, i, moves);
                    }
                    break;
                }
            }

            // 检查车的垂直移动向上
            for (int i = row - 1; i >= 0 ; i--) {
                if (board[i][col].type == ChessType::Empty) {
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
                if (board[i][col].type == ChessType::Empty) {
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

    void PaoRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
        // 检查炮的水平向左移动
        for (int i = col - 1; i >= 0; i--) {
            if (board[row][i].type == ChessType::Empty) {
                AddMoveIfValid(row, col, row, i, moves);
            } else if (board[row][i].type != Empty) {
                for (int j = i - 1; j >= 0; j--) {
                    if (board[row][j].type != Empty) {
                        AddMoveIfValid(row, col, row, j, moves);
                        break;
                    }
                }
                break;
            }
        }
        // 检查炮的水平向右移动
        for (int i = col + 1; i < 9; i++) {
            if (board[row][i].type == ChessType::Empty) {
                AddMoveIfValid(row, col, row, i, moves);
            } else if (board[row][i].type != Empty) {
                for (int j = i + 1; j < 9; j++) {
                    if (board[row][j].type != Empty) {
                        AddMoveIfValid(row, col, row, j, moves);
                        break;
                    }
                }
                break;
            }
        }
        // 检查炮的垂直向上移动
        for (int i = row - 1; i >= 0; i--) {
            if (board[i][col].type == ChessType::Empty) {
                AddMoveIfValid(row, col, i, col, moves);
            } else if (board[i][col].type != Empty) {
                for (int j = i - 1; j >= 0; j--) {
                    if (board[j][row].type != Empty) {
                        AddMoveIfValid(row, col, j, col, moves);
                        break;
                    }
                }
                break;
            }
        }
        // 检查炮的垂直向下移动
        for (int i = row + 1; i < 10; i++) {
            if (board[i][col].type == ChessType::Empty) {
                AddMoveIfValid(row, col, i, col, moves);
            } else if (board[i][col].type != Empty) {
                for (int j = i + 1; j < 10; j++) {
                    if (board[j][row].type != Empty) {
                        AddMoveIfValid(row, col, j, col, moves);
                        break;
                    }
                }
                break;
            }
        }
    }

    void MaRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
        // 八个可能的马脚位置
        int offsets[8][2] = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2},
                             {2, 1},  {2, -1},  {1, -2},  {-1, -2}};
        // 四个阻挡的位置
        int offsets2[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < 8; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            int mid_x = 0;
            int mid_y = 0;
            for (int j = 0; j < 4; j = i / 2) {
                mid_x = row + offsets2[j][0];
                mid_y = col + offsets2[j][1];
            }
            assert(0 <= end_x && end_x < 10);
            assert(0 <= end_y && end_y < 9);
            if (board[mid_x][mid_y].type == Empty && (board[end_x][end_y].type == Empty || board[end_x][end_y].is_red != board[row][col].is_red)) {
                AddMoveIfValid(row, col, end_x, end_y, moves);
            }
        }
    }

    void XiangRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
        // 四个可能的象眼的位置
        int offsets[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
        for (auto & offset : offsets) {
            int end_x = row + offset[0];
            int end_y = col + offset[1];
            assert(0 <= end_x && end_x < 10);
            assert(0 <= end_y && end_y < 9);
            if (board[row][col].is_red) {
                assert(4 < end_x && end_x< 10);
            } else {
                assert(0 <= end_x && end_y< 5);
            }
            int mid_x = (row + end_x) / 2;
            int mid_y = (col + end_y) / 2;
            if (board[mid_x][mid_y].type == Empty && (board[end_x][end_y].type == Empty || board[row][col].is_red != board[end_x][end_y].is_red)) {
                AddMoveIfValid(row, col, end_x, end_y, moves);
            }
        }
    }

    void ShiRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
        // 五个可能的士的位置
        int startCol = (col / 3) * 3;
        int startRow;
        if (0 <= row && row < 3) {
            startRow = (row / 3) * 3;
        } else {
            startRow = (row / 7) * 7;
        }
        for (int i = startRow; i < startRow + 3; i++) {
            for (int j = startCol; j < startCol + 3 ; j++) {
                int dist = (i - row) * (i - row) + (j - col) * (j - col);
                if (dist == 2 && (board[i][j].type == Empty || board[i][j].is_red != board[row][col].is_red)) {
                    AddMoveIfValid(startRow, startCol, row, col,moves);
                }
            }
        }
    }

    void WangRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
        // 四个可能的帅的移动位置
        int offsets[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            assert((0 <= end_x && end_x < 3) || (6 < end_x && end_x < 10));
            assert(2 < end_y && end_y < 6);
            if (board[end_x][end_y].type == Empty && board[end_x][end_y].is_red != board[row][col].is_red) {
                AddMoveIfValid(end_x, end_y, row, col, moves);
            }
        }
    }

    void BingRule(int row, int col, const Chess board[10][9], std::vector<ChessMove> *moves) {
        //兵的移动位置 判断是否过河;
        int ReGuohe[3][2] = {{-1, 0}, {0, 1}, {0, -1}};
        int BaGuohe[3][2] = {{0, -1}, {0, 1}, {1, 0}};
        for (int i = 0; i < 3; i++) {
            int end_x;
            int end_y;
            if (board[row][col].is_red) {
                if (row < 5) {
                    end_x = row + ReGuohe[i][0];
                    end_y = col + ReGuohe[i][1];
                    assert(0 <= end_x && end_x < 10);
                    assert(0 <= end_y && end_y < 9);
                    if (board[end_x][end_y].type == Empty && board[end_x][end_y].is_red != board[row][col].is_red) {
                        AddMoveIfValid(end_x, end_y, row, col, moves);
                    }
                } else {
                    end_x = row - 1;
                    end_y = col;
                    if (board[end_x][end_y].type == Empty && board[end_x][end_y].is_red != board[row][col].is_red) {
                        AddMoveIfValid(end_x, end_y, row, col, moves);
                    }
                }
            } else {
                if (row > 4) {
                    end_x = row + BaGuohe[i][0];
                    end_y = col + BaGuohe[i][1];
                    assert(0 <= end_x && end_x < 10);
                    assert(0 <= end_y && end_y < 9);
                    if (board[end_x][end_y].type == Empty && board[end_x][end_y].is_red != board[row][col].is_red) {
                        AddMoveIfValid(end_x, end_y, row, col, moves);
                    }
                } else {
                    end_x = row + 1;
                    end_y = col;
                    if (board[end_x][end_y].type == Empty && board[end_x][end_y].is_red != board[row][col].is_red) {
                        AddMoveIfValid(end_x, end_y, row, col, moves);
                    }
                }
            }
        }
    }



    void ChessBoard::GetMoves(std::vector<ChessMove> *moves) const {
        moves->reserve(100000);
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 9; col++) {
                switch ( board[row][col].type ) {
                    case Ju:
                        JuRule(row, col, board, moves);
                        break;
                    case Pao:
                        PaoRule(row, col, board, moves);
                        break;
                    case Ma:
                        MaRule(row, col, board, moves);
                        break;
                    case Xiang:
                        XiangRule(row, col, board, moves);
                        break;
                    case Shi:
                        ShiRule(row, col, board, moves);
                        break;
                    case Wang:
                        WangRule(row, col, board, moves);
                        break;
                    case Bing:
                        BingRule(row, col, board, moves);
                        break;
                    default:
                        break;
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
        return false;
    }

    BoardResult ChessBoard::End() const {
        return NOT_END;
    }

    void ChessBoard::ClearBoard() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                board[i][j].type = Empty;
            }
        }
    }

    void ChessBoard::initBoard() {
        ClearBoard();
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (i == 0 ) {
                    switch (j) {
                        case 0:
                            board[i][j].type = board[9 - i][j].type = board[i][8 - j].type = board[9 - i][8 -
                                                                                                          j].type = Ju;
                            board[i][j].is_red = board[i][8 - j].is_red = false;
                            board[9 - i][j].is_red = board[9 - i][8 - j].is_red = true;
                            break;
                        case 1:
                            board[i][j].type = board[9 - i][j].type = board[i][8 - j].type = board[9 - i][8 -
                                                                                                          j].type = Ma;
                            board[i][j].is_red = board[i][8 - j].is_red = false;
                            board[9 - i][j].is_red = board[9 - i][8 - j].is_red = true;
                            break;
                        case 2:
                            board[i][j].type = board[9 - i][j].type = board[i][8 - j].type = board[9 - i][8 -
                                                                                                          j].type = Xiang;
                            board[i][j].is_red = board[i][8 - j].is_red = false;
                            board[9 - i][j].is_red = board[9 - i][8 - j].is_red = true;
                            break;
                        case 3:
                            board[i][j].type = board[9 - i][j].type = board[i][8 - j].type = board[9 - i][8 -
                                                                                                          j].type = Shi;
                            board[i][j].is_red = board[i][8 - j].is_red = false;
                            board[9 - i][j].is_red = board[9 - i][8 - j].is_red = true;
                            break;
                        case 4:
                            board[i][j].type = board[9 - i][j].type = Wang;
                            board[i][j].is_red = false;
                            board[9 - i][j].is_red = true;
                            break;
                        default:
                            board[i][j].type = Empty;
                            board[i][j].is_red = true;
                            break;
                    }
                }
                else if (i == 2 && j == 1) {
                    board[i][j].type = board[9 - i][j].type = board[i][8 - j].type = board[9 - i][8 - j].type = Pao;
                    board[i][j].is_red = board[i][8-j].is_red = false;
                    board[9-i][j].is_red = board[9-i][8-j].is_red = true;
                }
            }
        }
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                if (j % 2 == 0) {
                    if (i == 3) {
                        board[i][j].type = Bing;
                        board[i][j].is_red = false;
                    }
                    else if (i == 6) {
                        board[i][j].type = Bing;
                        board[i][j].is_red = true;
                    }
                }
            }
        }
    }


    void ChessBoard::GetMovesFrom(int x, int y, std::vector<ChessMove> *move) const {

    }

    bool ChessBoard::SetChessAt(const Chess &chess, int x, int y) {
        return false;
    }

    void ChessBoard::ParseFromString(const std::string &str) {

    }

    std::string ChessBoard::ToString() const {
        return std::string();
    }

    void ChessBoard::PrintOnTerminal() {
        for (int i = 0; i < 10; ++i) {
            std::cout << std::endl;
            for (int j = 0; j < 9; ++j) {
                char c;
                switch (board[i][j].type) {
                    case Empty:
                        c = '0';
                        break;
                    default:
                        c = '1';
                        break;
                }
                std::cout << c << " ";
            }
        }

    }

    ChessBoard::ChessBoard() {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j].type = Empty;
                board[i][j].is_red = true;
            }
        }
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