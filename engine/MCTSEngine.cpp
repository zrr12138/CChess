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
#include "common/spinlock.h"

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
    MCTSEngine::MCTSEngine(int thread_num, double explore_c) : C(explore_c), thread_num_(thread_num), stop_(true),
                                                               actioning(false) {

    }

    bool MCTSEngine::StartSearch(const ChessBoard &state, bool red_first) {
        LOG(INFO) << __func__ << "thread_num_: " << thread_num_;
        stop_.store(false);
        LOG(INFO) << __func__ << " board: " << state.ToString() << " red_first: " << red_first;
        //初始化根节点x
        root_node_ = new Node(red_first, this);
        root_board_ = state;
        threadPool.Init(thread_num_, std::bind(&MCTSEngine::LoopExpandTree, this));
        threadPool.Start();
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
        threadPool.Stop();
        LOG(INFO) << "thread pool stop in "
                  << common::TimeUtility::GetTimeofDayMs() - start << " ms";
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
            }
        }
        LOG(WARNING) << "free node in " << common::TimeUtility::GetTimeofDayMs() - start << " ms";
        free(root_node_);
        root_node_ = new_root;
        assert(root_board_.Move(move));
        pause_cnt_ = 0;
        actioning = false;
        actioning.store(false);
        return false;
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
        bool is_red = node->is_red;
        if (node == nullptr || node->move_node_ == nullptr) {
            return;
        }
        auto move_node = std::max_element(node->move_node_, node->move_node_ + node->move_node_size_,
                                          [is_red](const std::pair<ChessMove, Node *> &x,
                                                   const std::pair<ChessMove, Node *> &y) -> bool {
                                              if (x.second == nullptr) {
                                                  return true;
                                              }
                                              return x.second->GetWinRate(is_red) < y.second->GetWinRate(is_red);
                                          });

        best_path->emplace_back(move_node->first, move_node->second->GetWinRate(is_red));
        getBestNode(move_node->second, best_path);
    }

    void MCTSEngine::FreeTree(Node *node) {
        if (node == nullptr) {
            return;
        }
        if (node->move_node_ == nullptr) {
            assert(node->move_node_size_ == 0);
            free(node);
            return;
        }
        for (int i = 0; i < node->move_node_size_; i++) {
            FreeTree(node->move_node_[i].second);
        }
        free(node);
    }

    BoardResult MCTSEngine::Simulation(const ChessBoard &board, bool is_red) {
        SearchCtx ctx;
        ctx.board = board;
        Node node(is_red, this);
        return node.Simulation(&ctx);
    }


    Node::Node(bool is_red, MCTSEngine *engine) : is_red(is_red), n(0), black_win_count(0), red_win_count(0),
                                                  engine_(engine), access_cnt(0), inited(false), move_node_(nullptr),
                                                  move_node_size_(0) {

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
        if (index >= move_node_size_) {
            std::pair<ChessMove, Node *> *move_node = nullptr;
            do {
                move_node = GetBestMove();
            } while (move_node == nullptr);
            LOG(INFO) << " node: " << this << " get best move:" << move_node->first << " next node "
                      << move_node->second;
            auto &move = move_node->first;
            auto &node = move_node->second;
            ctx->board.Move(move);
            auto res = node->ExpandTree(ctx);
            UpdateValue(res);
            return res;
        } else {
            assert(move_node_[index].second == nullptr);
            move_node_[index].second = new Node(!is_red, engine_);
            ctx->board.Move(move_node_[index].first);
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
        auto &board = ctx->board;
        int move_count_limit = 100;
        int move_count = 0;
        bool is_red_now = is_red;
        while (board.End() == BoardResult::NOT_END && move_count++ < move_count_limit) {
            assert(board.Move(board.RandMove2(is_red_now)));
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
        int64_t root_n = engine_->root_node_->n.load(std::memory_order::memory_order_relaxed);
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
            double ans = sub_w / sub_n + engine_->C * std::sqrt(ln_root_n / sub_n);
            if (ans > max) {
                max = ans;
                ptr = move_node_ + i;
            }
        }
        return ptr;
    }
}
