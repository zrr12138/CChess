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

    void ChessBoard::JuRule(int row, int col,std::vector<ChessMove> *moves) const{
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

    void ChessBoard::PaoRule(int row, int col, std::vector<ChessMove> *moves) const{
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
                    if (board[j][col].type != Empty && board[row][col].is_red != board[j][col].is_red) {
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
                    if (board[j][col].type != Empty) {
                        AddMoveIfValid(row, col, j, col, moves);
                        break;
                    }
                }
                break;
            }
        }
    }

    void ChessBoard::MaRule(int row, int col, std::vector<ChessMove> *moves) const{
        // 八个可能的马脚位置
        int offsets[8][2] = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2},
                             {2, 1},  {2, -1},  {1, -2},  {-1, -2}};
        // 四个阻挡的位置
        int offsets2[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < 8; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            int mid_x = row + offsets2[i / 2][0];
            int mid_y = col + offsets2[i / 2][1];
            if (0<= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                if (board[mid_x][mid_y].type == Empty && (board[end_x][end_y].type == Empty || board[end_x][end_y].is_red != board[row][col].is_red)) {
                    AddMoveIfValid(row, col, end_x, end_y, moves);
                }
            }
        }
    }

    void ChessBoard::XiangRule(int row, int col, std::vector<ChessMove> *moves) const{
        // 四个可能的象眼的位置
        int offsets[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            int mid_x = (row + end_x) / 2;
            int mid_y = (col + end_y) / 2;
            if (0 <= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                if ((row < 5 && end_x < 5) || (row >= 5 && end_x >= 5)) {
                    if (board[mid_x][mid_y].type == Empty && (board[end_x][end_y].type == Empty || board[row][col].is_red != board[end_x][end_y].is_red)) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                    }
                }
            }
        }
    }

    void ChessBoard::ShiRule(int row, int col, std::vector<ChessMove> *moves) const{
        // 五个可能的士的位置
        int offsets[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            if (3 <= end_y && end_y <= 5) {
                if ((end_x < 3 && end_x >= 0) || (end_x > 6 && end_x <= 9)) {
                    if ((board[end_x][end_y].type == Empty || board[row][col].is_red != board[end_x][end_y].is_red)) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);

                    }
                }
            }
        }
    }

    void ChessBoard::WangRule(int row, int col, std::vector<ChessMove> *moves) const{
        // 四个可能的帅的移动位置
        int offsets[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < 4; i++) {
            int end_x = row + offsets[i][0];
            int end_y = col + offsets[i][1];
            if (3 <= end_y && end_y <= 5) {
                if ((end_x < 3 && end_x >= 0) || (end_x > 6 && end_x <= 9)) {
                    if ((board[end_x][end_y].type == Empty || board[row][col].is_red != board[end_x][end_y].is_red)) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                    }
                }
            }
        }
    }

    void ChessBoard::BingRule(int row, int col, std::vector<ChessMove> *moves) const{
        //兵的移动位置 判断是否过河;
        int ReGuohe[3][2] = {{-1, 0}, {0, 1}, {0, -1}};
        int BaGuohe[3][2] = {{0, -1}, {0, 1}, {1, 0}};
        for (int i = 0; i < 3; i++) {
            int end_x;
            int end_y;
            if ((board[row][col].is_red && board_red) || (!board[row][col].is_red && !board_red)) {
                if (row < 5) {
                    end_x = row + ReGuohe[i][0];
                    end_y = col + ReGuohe[i][1];
                    if (0<= end_x && end_x < 10 && 0 <= end_y && end_y < 9){
                        if (board[end_x][end_y].type == Empty || board[end_x][end_y].is_red != board[row][col].is_red) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                        }
                    }
                } else {
                    end_x = row - 1;
                    end_y = col;
                    if (board[end_x][end_y].type == Empty || board[end_x][end_y].is_red != board[row][col].is_red) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                        break;
                    }
                }
            } else if((!board[row][col].is_red && board_red) || (board[row][col].is_red && !board_red)) {
                if (row > 4) {
                    end_x = row + BaGuohe[i][0];
                    end_y = col + BaGuohe[i][1];
                    if (0<= end_x && end_x < 10 && 0 <= end_y && end_y < 9) {
                        if (board[end_x][end_y].type == Empty || board[end_x][end_y].is_red != board[row][col].is_red) {
                            AddMoveIfValid(row, col, end_x, end_y, moves);
                        }
                    }

                } else {
                    end_x = row + 1;
                    end_y = col;
                    if (board[end_x][end_y].type == Empty || board[end_x][end_y].is_red != board[row][col].is_red) {
                        AddMoveIfValid(row, col, end_x, end_y, moves);
                        break;
                    }
                }
            }
        }
    }



    void ChessBoard::GetMoves(std::vector<ChessMove> *moves) const{
        //moves->reserve(1);
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 9; col++) {
                switch ( board[row][col].type ) {
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

    Chess ChessBoard::GetChessAt(int x, int y) const {
        assert(0 <= x && x < 10);
        assert(0 <= x && x < 9);
        return {board[x][y].type, board[x][y].is_red};
    }

    bool ChessBoard::Move(const ChessMove &move, std::vector<Chess> *dead) {
        assert(0 <= move.end_x && move.end_x < 10);
        assert(0 <= move.end_x && move.end_y < 9);
        auto &chess= board[move.end_x][move.end_y];
        if (chess.is_red == board[move.start_x][move.start_y].is_red ) {
//            LOG(ERROR) << "move failed, move: " << move;
            return false;
        }
        if (chess.type != Empty) {
            dead->emplace_back(chess);
        }
        is_init = false;
        std::cout << is_end << std::endl;
        update_is_end_from(move);
        assert(is_end == 2);
        move_num++;
        return true;
    }

    BoardResult ChessBoard::End() const {
        switch (is_end) {
            case 2:
                return BoardResult::NOT_END;
            case 1:
                return BoardResult::BLACK_WIN;
            case 0:
                return BoardResult::RED_WIN;
            default:
                assert(2);
        }
    }

    void ChessBoard::ClearBoard() {
        is_end = 2;
        move_num = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                board[i][j].type = Empty;
            }
        }
    }

    void ChessBoard::initBoard() {
        ClearBoard();
        if (board_red) {
            SetChessAt(Chess(ChessType::Ju, true),9, 0);
            SetChessAt(Chess(ChessType::Ma, true),9, 1);
            SetChessAt(Chess(ChessType::Xiang, true),9, 2);
            SetChessAt(Chess(ChessType::Shi, true),9, 3);
            SetChessAt(Chess(ChessType::Wang, true),9, 4);
            SetChessAt(Chess(ChessType::Shi, true),9, 5);
            SetChessAt(Chess(ChessType::Xiang, true),9, 6);
            SetChessAt(Chess(ChessType::Ma, true),9, 7);
            SetChessAt(Chess(ChessType::Ju, true),9, 8);
            SetChessAt(Chess(ChessType::Pao, true),7, 1);
            SetChessAt(Chess(ChessType::Pao, true),7, 7);
            SetChessAt(Chess(ChessType::Bing, true),6, 0);
            SetChessAt(Chess(ChessType::Bing, true),6, 2);
            SetChessAt(Chess(ChessType::Bing, true),6, 4);
            SetChessAt(Chess(ChessType::Bing, true),6, 6);
            SetChessAt(Chess(ChessType::Bing, true),6, 8);

            SetChessAt(Chess(ChessType::Ju, false),0, 0);
            SetChessAt(Chess(ChessType::Ma, false),0, 1);
            SetChessAt(Chess(ChessType::Xiang, false),0, 2);
            SetChessAt(Chess(ChessType::Shi, false),0, 3);
            SetChessAt(Chess(ChessType::Wang, false),0, 4);
            SetChessAt(Chess(ChessType::Shi, false),0, 5);
            SetChessAt(Chess(ChessType::Xiang, false),0, 6);
            SetChessAt(Chess(ChessType::Ma, false),0, 7);
            SetChessAt(Chess(ChessType::Ju, false),0, 8);
            SetChessAt(Chess(ChessType::Pao, false),2, 1);
            SetChessAt(Chess(ChessType::Pao, false),2, 7);
            SetChessAt(Chess(ChessType::Bing, false),3, 0);
            SetChessAt(Chess(ChessType::Bing, false),3, 2);
            SetChessAt(Chess(ChessType::Bing, false),3, 4);
            SetChessAt(Chess(ChessType::Bing, false),3, 6);
            SetChessAt(Chess(ChessType::Bing, false),3, 8);
        } else {
            SetChessAt(Chess(ChessType::Ju, false),9, 0);
            SetChessAt(Chess(ChessType::Ma, false),9, 1);
            SetChessAt(Chess(ChessType::Xiang, false),9, 2);
            SetChessAt(Chess(ChessType::Shi, false),9, 3);
            SetChessAt(Chess(ChessType::Wang, false),9, 4);
            SetChessAt(Chess(ChessType::Shi, false),9, 5);
            SetChessAt(Chess(ChessType::Xiang, false),9, 6);
            SetChessAt(Chess(ChessType::Ma, false),9, 7);
            SetChessAt(Chess(ChessType::Ju, false),9, 8);
            SetChessAt(Chess(ChessType::Pao, false),7, 1);
            SetChessAt(Chess(ChessType::Pao, false),7, 7);
            SetChessAt(Chess(ChessType::Bing, false),6, 0);
            SetChessAt(Chess(ChessType::Bing, false),6, 2);
            SetChessAt(Chess(ChessType::Bing, false),6, 4);
            SetChessAt(Chess(ChessType::Bing, false),6, 6);
            SetChessAt(Chess(ChessType::Bing, false),6, 8);

            SetChessAt(Chess(ChessType::Ju, true),0, 0);
            SetChessAt(Chess(ChessType::Ma, true),0, 1);
            SetChessAt(Chess(ChessType::Xiang, true),0, 2);
            SetChessAt(Chess(ChessType::Shi, true),0, 3);
            SetChessAt(Chess(ChessType::Wang, true),0, 4);
            SetChessAt(Chess(ChessType::Shi, true),0, 5);
            SetChessAt(Chess(ChessType::Xiang, true),0, 6);
            SetChessAt(Chess(ChessType::Ma, true),0, 7);
            SetChessAt(Chess(ChessType::Ju, true),0, 8);
            SetChessAt(Chess(ChessType::Pao, true),2, 1);
            SetChessAt(Chess(ChessType::Pao, true),2, 7);
            SetChessAt(Chess(ChessType::Bing, true),3, 0);
            SetChessAt(Chess(ChessType::Bing, true),3, 2);
            SetChessAt(Chess(ChessType::Bing, true),3, 4);
            SetChessAt(Chess(ChessType::Bing, true),3, 6);
            SetChessAt(Chess(ChessType::Bing, true),3, 8);
        }

    }


    void ChessBoard::GetMovesFrom(int x, int y, std::vector<ChessMove> *move) const {
        GetMoves(move);
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
        board[x][y] = chess;
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

    void ChessBoard::PrintMoves(std::vector<ChessMove> *moves) const{
            for (const ChessMove& move : *moves) {
                std::cout << "StartX: " << move.start_x << ", StartY: " << move.start_y
                          << ", EndX: " << move.end_x << ", EndY: " << move.end_y << std::endl;
            }
    }

    void ChessBoard::update_is_end_from(ChessMove move) {
        //auto &chess = board[move.end_x][move.end_y];
        if (board[move.end_x][move.end_y].type == Wang) {
            if (board[move.end_x][move.end_y].is_red) {
                is_end = 1;
            } else {
                is_end = 0;
            }
        }
        board[move.end_x][move.end_y] = board[move.start_x][move.start_y];
        board[move.start_x][move.start_y].type = Empty;
    }

    void ChessBoard::BoardRed(bool is_red) {
        board_red = is_red;
    }

    void ChessBoard::PrintDead(std::vector<Chess> *deads) const {
        for (const Chess& dead : *deads) {
            std::cout << "type: " << dead.type << ", Color: " << dead.is_red << std::endl;
        }
    }

    bool legal_shi(int i, int j) {
        if (j == 4) {
            if(i == 0 || i == 2 || i == 7 || i ==9)
                return false;
        } else if (j == 3) {
            if (i == 1 || i == 8)
                return false;
        } else if (j == 5) {
            if (i == 1 || i == 8)
                return false;
        }
        return true;
    }

    bool ChessBoard::Legal() const {
        int num_rema = 0;
        int num_reju = 0;
        int num_repao = 0;
        int num_rexiang = 0;
        int num_reshi = 0;
        int num_rebing = 0;
        int num_rewang = 0;
        int num_bama = 0;
        int num_baju = 0;
        int num_bapao = 0;
        int num_baxiang = 0;
        int num_bashi = 0;
        int num_babing = 0;
        int num_bawang = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                switch (board[i][j].type) {
                    case Ju:
                        if (board[i][j].is_red)
                            num_reju++;
                        else
                            num_baju++;
                        break;
                    case Wang:
                        if (board[i][j].is_red)
                            num_rewang++;
                        else
                            num_bawang++;
                        break;
                    case Ma:
                        if (board[i][j].is_red)
                            num_rema++;
                        else
                            num_bama++;
                        break;
                    case Bing:
                        if (board[i][j].is_red)
                            num_rebing++;
                        else
                            num_babing++;
                        break;
                    case Shi:
                        if (board[i][j].is_red)
                            num_reshi++;
                        else
                            num_bashi++;
                        break;
                    case Pao:
                        if (board[i][j].is_red)
                            num_repao++;
                        else
                            num_bapao++;
                        break;
                    case Xiang:
                        if (board[i][j].is_red)
                            num_rexiang++;
                        else
                            num_baxiang++;
                        break;
                    default:
                        break;
                }
            }
        }
        if (num_reju > 2 || num_baju > 2 || num_bama > 2 || num_rema > 2 || num_repao > 2 || num_bapao > 2) {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                switch (board[i][j].type) {
                    case Wang:

                        break;
                    case Bing:
                        break;
                    case Shi:
                        if (j == 4) {
                            if(i == 0 || i == 2 || i == 7 || i ==9)
                                return false;
                        } else if (j == 3) {
                            if (i == 1 || i == 8)
                                return false;
                        } else if (j == 5) {
                            if (i == 1 || i == 8)
                                return false;
                        }
                        break;
                    case Xiang:
                        break;
                    default:
                        break;
                }
            }
        }
        return true;
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