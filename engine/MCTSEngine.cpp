//
// Created by zhengran on 2024/2/8.
//

#include "MCTSEngine.h"
#include <iostream>
#include <memory>
#include <stack>
#include "glog/logging.h"
#include "common/timeutility.h"
#include "common/defer.h"
#include "common/rw_lock.h"
#include <cmath>
#include <chrono>
#include <thread>
#include <cassert>
#include <random>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <iomanip>
#include "common/defer.h"
#include "common/spinlock.h"
#include "common/defer.h"
#include "WeightedRandomSelector.h"
#include "gflags/gflags.h"

DEFINE_int32(thread_num, 8, "");
DEFINE_double(explore_c, sqrt(2.7), "");

#define THROTTLE_CALL(delay) \
    do { \
        static uint64_t last_call_timestamp = 0; \
        uint64_t now = common::TimeUtility::GetTimeofDayMs(); \
        if (now - last_call_timestamp < (delay)) { \
            std::this_thread::sleep_for(std::chrono::milliseconds((delay) - (now - last_call_timestamp))); \
        } \
        last_call_timestamp = now; \
    } while(0)
namespace CChess {


    bool MCTSEngine::StartSearch(const ChessBoard &state, bool red_first) {
        if (!stop_) {
            return false;
        }
        LOG(INFO) << __func__ << "thread_num_: " << thread_num_;
        stop_.store(false);
        LOG(INFO) << __func__ << " board: " << state.ToString() << " red_first: " << red_first;
        //初始化根节点x
        root_node_ = new Node(red_first, nullptr, state, ChessMove());
        root_board_ = state;
        assert(threadPool == nullptr);
        threadPool = new common::ThreadPool();
        threadPool->Init(thread_num_, std::bind(&MCTSEngine::LoopExpandTree, this));
        threadPool->Start();
        pause_cnt_ = 0;
        return true;
    }

    void MCTSEngine::LoopExpandTree() {
        LOG(WARNING) << "start loop expand tree";
        while (!stop_.load()) {
            if (actioning.load()) {
                pause_cnt_++;
                LOG(INFO) << __func__ << "wait action pause cnt:" << pause_cnt_ << " thread num:" << thread_num_;
                while (actioning.load());
            }
            SearchCtx ctx;
            ctx.board = root_board_;
            root_node_->ExpandTree(&ctx);
        }
    }


    bool MCTSEngine::Stop() {
        stop_.store(true);
        auto start = common::TimeUtility::GetTimeofDayMs();
        if (threadPool != nullptr) {
            threadPool->Stop();
        }
        LOG(WARNING) << "thread pool stop in "
                     << common::TimeUtility::GetTimeofDayMs() - start << " ms";
        FreeTree(root_node_);
        root_node_ = nullptr;
        delete threadPool;
        threadPool = nullptr;
        return true;
    }

    bool MCTSEngine::Action(ChessMove move) {
        THROTTLE_CALL(1000);

        assert(root_node_->move_node_ != nullptr);
        assert(root_node_->move_node_size_ > 0);
        assert(root_board_.End() == BoardResult::NOT_END);

        Node *new_root = nullptr;
        for (int i = 0; i < root_node_->move_node_size_; i++) {
            if (root_node_->move_node_[i].first == move) {
                new_root = root_node_->move_node_[i].second;
                break;
            }
        }
        if (new_root == nullptr) {
            return false;
        }
        LOG(WARNING) << __func__ << " move: " << move;
        actioning.store(true);
        while (pause_cnt_ != thread_num_);
        // free node
        auto start = common::TimeUtility::GetTimeofDayMs();

        for (int i = 0; i < root_node_->move_node_size_; i++) {
            if (root_node_->move_node_[i].first != move) {
                FreeTree(root_node_->move_node_[i].second);
                root_node_->move_node_[i].second = nullptr;
            }
        }
        LOG(WARNING) << "free node in " << common::TimeUtility::GetTimeofDayMs() - start << " ms";
        NodeHashManager::getInstance().Remove(root_node_->node_hash);
        delete root_node_;
        root_node_ = new_root;
        root_node_->father = nullptr;
        assert(root_board_.Move(move));
        pause_cnt_ = 0;
        actioning = false;
        actioning.store(false);
        if (root_board_.End() != BoardResult::NOT_END) {
            Stop();
        }
        return true;
    }

    ChessMove MCTSEngine::GetResult() {
        assert(actioning.load() == false); // engine 不支持多线程操作
        THROTTLE_CALL(1000);
        bool is_red = root_node_->is_red;
        auto move = std::max_element(root_node_->move_node_, root_node_->move_node_ + root_node_->move_node_size_,
                                     [is_red](const std::pair<ChessMove, Node *> &x,
                                              const std::pair<ChessMove, Node *> &y) -> bool {
                                         return x.second->GetWinRate(is_red) < y.second->GetWinRate(is_red);
                                     })->first;
        return move;
    }

//    void MCTSEngine::DumpTree() {
//        std::ofstream outputFile("tree.txt");
//        outputFile << "root_n:" << root_node_->n << std::endl;
//        PrintNode(outputFile, root_node_.get(), ChessMove(), 0);
//        outputFile.close();
//    }

//    void MCTSEngine::PrintNode(std::ostream &os, Node *node, ChessMove move, int deep) {
//        os << "\n";
//        for (int i = 0; i < deep; i++) {
//            os << "\t";
//        }
//        os << move;
//        os << " value:" << node->GetValue() << " b_win rate:"
//           << node->GetWinRate(true) << " w_win rate:" << node->GetWinRate(false) << " bwc:" << node->black_win_count
//           << " wwc" << node->white_win_count << " n:" << node->n;
//        for (auto &move_node: node->move2node_) {
//            PrintNode(os, move_node.second.get(), move_node.first, deep + 1);
//        }
//    }

    int64_t MCTSEngine::GetRootN() {
        assert(actioning.load() == false); // engine 不支持多线程操作
        return root_node_->n;
    }

    void MCTSEngine::GetBestPath(std::vector<std::pair<ChessMove, double>> *best_path) {
        assert(actioning.load() == false); // engine 不支持多线程操作
        getBestNode(root_node_, best_path);
    }

    void MCTSEngine::getBestNode(Node *node, std::vector<std::pair<ChessMove, double>> *best_path) {
        if (node == nullptr) {
            return;
        }
        {
            node->initLock.Lock();
            defer({
                      node->initLock.UnLock();
                  });
            if (node->move_node_ == nullptr) {
                return;
            }
        }

        bool is_red = node->is_red;
        bool have_nullptr = false;
        auto move_node = std::max_element(node->move_node_, node->move_node_ + node->move_node_size_,
                                          [&](const std::pair<ChessMove, Node *> &x,
                                              const std::pair<ChessMove, Node *> &y) -> bool {
                                              if (x.second == nullptr || y.second == nullptr) {
                                                  have_nullptr = true;
                                                  return true;
                                              }
                                              return x.second->GetWinRate(is_red) < y.second->GetWinRate(is_red);
                                          });
        if (have_nullptr) {
            return;
        }
        best_path->emplace_back(move_node->first, move_node->second->GetWinRate(is_red));
        getBestNode(move_node->second, best_path);
    }

    void MCTSEngine::FreeTree(Node *node) {
        if (node == nullptr) {
            return;
        }
        defer({
                  NodeHashManager::getInstance().Remove(root_node_->node_hash);
                  delete node;
                  node = nullptr;
              });
        if (node->move_node_ == nullptr) {
            assert(node->move_node_size_ == 0);
            return;
        }
        for (int i = 0; i < node->move_node_size_; i++) {
            FreeTree(node->move_node_[i].second);
            node->move_node_[i].second = nullptr;
        }
    }

    BoardResult MCTSEngine::Simulation(const ChessBoard &board, bool is_red) {
        SearchCtx ctx;
        ctx.board = board;
        Node node(is_red, nullptr, board, ChessMove());
        return node.Simulation(&ctx);
    }

    bool MCTSEngine::IsRunning() {
        return !stop_;
    }

    const ChessBoard &MCTSEngine::GetChessBoard() const {
        return root_board_;

    }

    MCTSEngine::~MCTSEngine() {
        Stop();

    }

    std::string MCTSEngine::GetRootChooseStr() const {
        THROTTLE_CALL(1000);
        struct NodeInfo {
            std::string move;
            int64_t n{}, red_win_count{}, black_win_count{};
        };
        int n = root_node_->move_node_size_;
        bool is_red = root_node_->is_red;
        NodeInfo info[root_node_->move_node_size_];
        {
            for (int i = 0; i < root_node_->move_node_size_; i++) {
                assert(GetChessBoard().MoveConversion(root_node_->move_node_[i].first, &info[i].move));
                info[i].n = root_node_->move_node_[i].second->n;
                info[i].black_win_count = root_node_->move_node_[i].second->black_win_count;
                info[i].red_win_count = root_node_->move_node_[i].second->red_win_count;
            }
        }

        std::sort(info, info + n, [is_red](const NodeInfo &x, const NodeInfo &y) -> bool {
            if (is_red) {
                return 1.0 * x.red_win_count / x.n > 1.0 * y.red_win_count / y.n;
            } else {
                return 1.0 * x.black_win_count / x.n > 1.0 * y.black_win_count / y.n;
            }
        });
        const std::string space = "          ";
        std::stringstream ss;
        ss << "move" << space << "n" << space << "red_win" << space << "black win" << space << "red rate" << space
           << "black rate" << "\n";
        ss << std::fixed << std::setprecision(2);
        for (int i = 0; i < n; i++) {
            ss << info[i].move << space << info[i].n << space << info[i].red_win_count << space
               << info[i].black_win_count << space << 1.0 * info[i].red_win_count / info[i].n << space
               << 1.0 * info[i].black_win_count / info[i].n << "\n";
        }
        return ss.str();
    }


    Node::Node(bool is_red, Node *father, const ChessBoard &board, const ChessMove &move)
            : is_red(is_red), n(0), black_win_count(0), red_win_count(0),
              access_cnt(0), inited(false), move_node_(nullptr),
              move_node_size_(0), father(father), move_(move), is_repeat(false) {
        if (father == nullptr) {
            ChessBoard::Hash hash_fun;
            board_hash = hash_fun(board);
        } else {
            board_hash = ChessBoard::Hash::getNextHash(father->board_hash, board, move);
        }
        node_hash = NodeHashManager::GetNodeHash(board_hash, is_red);
        if (NodeHashManager::getInstance().Get(node_hash)) {
            SetRepeat();
        }
        NodeHashManager::getInstance().Add(node_hash);

    }

    void Node::UpdateValue(BoardResult res) {
        n.fetch_add(1, std::memory_order::memory_order_relaxed);
        if (res == BoardResult::RED_WIN) {
            red_win_count.fetch_add(1, std::memory_order::memory_order_relaxed);
        } else if (res == BoardResult::BLACK_WIN) {
            black_win_count.fetch_add(1, std::memory_order::memory_order_relaxed);
        }
    }

    BoardResult Node::ExpandTree(SearchCtx *ctx) {
        if (ctx->board.End() != BoardResult::NOT_END) {
            UpdateValue(ctx->board.End());
            return ctx->board.End();
        }
        int64_t index = access_cnt.fetch_add(1);
        LOG(INFO) << __func__ << " node: " << this << " access index:" << index;
        if (!inited) {
            Init(ctx->board);
        }
        if (move_node_size_ == 0) {
            auto end = is_red ? BLACK_WIN : RED_WIN;
            UpdateValue(end);
            return end;
        }
        if (index >= move_node_size_) {
            std::pair<ChessMove, Node *> *move_node = nullptr;
            do {
                move_node = GetBestMove();
            } while (move_node == nullptr);
            LOG(INFO) << " node: " << this << " get best move:" << move_node->first << " next node "
                      << move_node->second;
            auto &move = move_node->first;
            auto &node = move_node->second;
            assert(ctx->board.Move(move));
            auto res = node->ExpandTree(ctx);
            UpdateValue(res);
            return res;
        } else {
            assert(move_node_[index].second == nullptr);
            const ChessMove &move = move_node_[index].first;
            move_node_[index].second = new Node(!is_red, this, ctx->board, move);
            assert(ctx->board.Move(move));
            LOG(INFO) << __func__ << " node: " << this << " expand node: " << move_node_[index].second << " with "
                      << move_node_[index].first << " start simulation";
            auto res = move_node_[index].second->Simulation(ctx);
            LOG(INFO) << __func__ << " node: " << this << " expand node: " << move_node_[index].second
                      << " simulation end with " << getBoardResultStr(res);
            UpdateValue(res);
            return res;
        }
    }

    BoardResult Node::Simulation(SearchCtx *ctx) {
        auto convert2DTo1D = [](int row, int col) -> int {
            const int numCols = 9;
            return row * numCols + col;
        };

        auto convert1DTo2D = [](int index, int &row, int &col) -> void {
            const int numCols = 9;
            row = index / numCols;
            col = index % numCols;
        };
//        Wang = 0,
//        Ma = 1,
//        Bing = 2,
//        Shi = 3,
//        Ju = 4,
//        Pao = 5,
//        Xiang = 6,
        ChessType type;
        int type2weight[] = {2, 8, 3, 2, 17, 17, 3};
        thread_local int weights[90] = {0};
        auto &board = ctx->board;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                int index = convert2DTo1D(i, j);
                auto chess = board.GetChessAt(i, j);
                weights[index] = chess.type == ChessType::Empty ? 0 : type2weight[chess.type];
            }
        }

        WeightedRandomSelector selector(weights, 90);
        thread_local std::minstd_rand generator{std::random_device{}()};
        thread_local std::uniform_int_distribution<int> dist(1, std::numeric_limits<int>::max());

        int move_count_limit = 100;
        int move_count = 0;
        bool is_red_now = is_red;
        std::vector<ChessMove> temp;
        while (board.End() == BoardResult::NOT_END && move_count++ < move_count_limit) {

            int index;
            do {
                index = selector.getRandomIndex();
                int row, col;
                convert1DTo2D(index, row, col);
                temp.clear();
                board.GetMovesFrom(row, col, &temp);
            } while (temp.empty());

            auto &move = temp[dist(generator) % temp.size()];
            assert(board.Move(move));
            int new_index = convert2DTo1D(move.end_x, move.end_y);
            selector.UpdateWeight(new_index, type2weight[board.GetChessAt(move.end_x, move.end_y).type]);
            selector.UpdateWeight(index, 0);
            is_red_now = !is_red_now;
        }
        BoardResult end = board.End();
        UpdateValue(end);
        return end;
    }


    double Node::GetWinRate(bool red_rate) {
        double dw, dn;
        {
            if (red_rate) {
                dw = static_cast<double >(red_win_count.load(std::memory_order::memory_order_relaxed));
            } else {
                dw = static_cast<double >(black_win_count.load(std::memory_order::memory_order_relaxed));
            }
            dn = static_cast<double >(n.load(std::memory_order::memory_order_relaxed));
            if (dn == 0) {
                return 0;
            }
        }
        return dw / dn;
    }

    Node::~Node() {
        if (move_node_) {
            delete[] move_node_;
        }
    }

    void Node::Init(const ChessBoard &board) {
        initLock.Lock();
        if (inited) {
            initLock.UnLock();  // 在返回之前解锁
            return;
        }

        thread_local std::vector<ChessMove> moves;
        moves.clear();
        board.GetMoves(is_red, &moves);
        assert(move_node_ == nullptr);
        //FilterByRule 按照规则过滤不能走的棋
        FilterByRule(board, &moves);
        //
        move_node_size_ = moves.size();
        move_node_ = new std::pair<ChessMove, Node *>[move_node_size_];
        for (int i = 0; i < move_node_size_; i++) {
            move_node_[i].first = moves[i];
            move_node_[i].second = nullptr;
        }
        inited = true;
        LOG(INFO) << __func__ << " node: " << this << "  init finish" << " move size:" << moves.size();
        initLock.UnLock();
    }

    std::pair<ChessMove, Node *> *Node::GetBestMove() {
        assert(move_node_ != nullptr);
        assert(move_node_size_ > 0);
        int64_t root_n = MCTSEngine::getInstance().root_node_->n.load(std::memory_order::memory_order_relaxed);
        double ln_root_n = std::log(root_n);
        double max = 0;
        std::pair<ChessMove, Node *> *ptr = nullptr;
        for (int i = 0; i < move_node_size_; i++) {
            if (move_node_[i].second == nullptr ||
                move_node_[i].second->n.load(std::memory_order::memory_order_relaxed) == 0)
                continue;
            double sub_n = move_node_[i].second->n.load(std::memory_order::memory_order_relaxed);
            double sub_w;
            if (is_red) {
                sub_w = move_node_[i].second->red_win_count.load(std::memory_order::memory_order_relaxed);
            } else {
                sub_w = move_node_[i].second->black_win_count.load(std::memory_order::memory_order_relaxed);
            }
            double ans = sub_w / sub_n + MCTSEngine::getInstance().C * std::sqrt(ln_root_n / sub_n);
            if (ans > max) {
                max = ans;
                ptr = move_node_ + i;
            }
        }
        return ptr;
    }

    void Node::SetRepeat() {
        is_repeat = true;
        //n = 10000;
    }

    void Node::FilterByRule(const ChessBoard &board, std::vector<ChessMove> *moves) {
        if (!IsRepeat()) {
            return;
        }
        std::vector<ChessMove> cycle_moves;
        bool find_cycle = false;
        Node *node = this;
        while (node->father != nullptr) {
            cycle_moves.emplace_back(node->move_);
            node = node->father;
            if (node->node_hash == this->node_hash) {
                find_cycle = true;
                break;
            }
        }
        if (!find_cycle) {
            return;
        }
        //TODO(zrr) 对于同一个循环，这里可能会重复计算
        std::reverse(cycle_moves.begin(), cycle_moves.end());
        std::vector<int> score;
        std::vector<ChessMove> temp;
        auto temp_board = board;
        auto temp_is_red = is_red;
        static int type2ruleScore[] = {1000, 45, 10, 5, 90, 45, 10};
        for (auto &cycle_move: cycle_moves) {
            int this_move_score = 0;
            assert(temp_board.Move(cycle_move));
            temp.clear();
            temp_board.GetMoves(temp_is_red, &temp); //下一步是否能杀王
            for (auto &it: temp) {
                auto &c = temp_board.GetChessAt(it.end_x, it.end_y);
                if (c.type == ChessType::Wang && c.is_red == !temp_is_red) {
                    this_move_score = 1000;
                    goto next_cycle_move;
                }
            }
            temp.clear();
            temp_board.GetMovesFrom(cycle_move.end_x, cycle_move.end_y, &temp);
            for (auto &it: temp) {
                auto &c = temp_board.GetChessAt(it.end_x, it.end_y);
                if (c.is_red == !temp_is_red) {
                    this_move_score = type2ruleScore[c.type];
                }
            }
            next_cycle_move:
            score.emplace_back(this_move_score);
            temp_is_red = !temp_is_red;
        }
        auto max_index = std::max_element(score.begin(), score.end()) - score.begin();
        if (max_index == 0) {
            bool is_cut = true;
            for (int i = 1; i < score.size(); i += 2) {
                if (score[0] - score[i] < 39) {
                    is_cut = false;
                    break;
                }
            }
            if (is_cut) {
                auto it = std::find(moves->begin(), moves->end(), cycle_moves[0]);
                assert(it != moves->end());
                moves->erase(it);
            }
        }
    }

    bool Node::IsRepeat() {
        return is_repeat;
    }

    size_t NodeHashManager::GetNodeHash(size_t board_hash, bool is_red) {
        if (is_red) {
            return ~board_hash;
        }
        return board_hash;
    }

    void NodeHashManager::Add(size_t node_hash) {
        lock_.Lock();
        hash2cnt[node_hash]++;
        lock_.UnLock();
    }

    void NodeHashManager::Remove(size_t node_hash) {
        lock_.Lock();
        if (--hash2cnt[node_hash] == 0) {
            hash2cnt.erase(node_hash);
        }
        lock_.UnLock();
    }

    int NodeHashManager::Get(size_t node_hash) {
        lock_.Lock();
        int temp = hash2cnt[node_hash];
        lock_.UnLock();
        return temp;
    }
}
