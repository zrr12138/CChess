
#include <queue>
#include "common/task_thread_pool.h"
#include "common/thread_pool.h"
#include <cmath>
#include <list>
#include "ChessBoard.h"
#include "common/rw_lock.h"
#include "common/spinlock.h"

#ifndef CCHESS_MCTSENGINE_H
#define CCHESS_MCTSENGINE_H

namespace CChess {
    struct SearchCtx;

    class MCTSEngine;

    struct Node {
        Node(bool is_red, MCTSEngine *engine);
        ~Node();
        std::atomic<int64_t> n, red_win_count, black_win_count;
        std::atomic<int64_t> access_cnt;

        bool inited;
        common::SpinLock initLock;

        std::pair<ChessMove, Node*> *move_node_;
        int move_node_size_;

        bool is_red;
        MCTSEngine *engine_;

        void UpdateValue(BoardResult res);

        double GetWinRate(bool red_rate);

        std::pair<ChessMove, Node*> *GetBestMove();

        BoardResult ExpandTree(SearchCtx *ctx);
        BoardResult Simulation(SearchCtx *ctx);
        void Init(const ChessBoard &board);
    };

    struct SearchCtx {
        ChessBoard board;
    };

    class MCTSEngine {
        friend Node;
    public:
        explicit MCTSEngine(int thread_num, double explore_c = std::sqrt(2));

        bool StartSearch(const ChessBoard &state, bool red_first);

        bool Action(ChessMove move);

        ChessMove GetResult(); //获取搜索结果,该函数不应该中断搜索，可以反复调用获取最新的搜索结果

        bool Stop();

        BoardResult Simulation(const ChessBoard &board, bool is_red); //for unit test

//        void DumpTree();

        int64_t GetRootN();

        void GetBestPath(std::vector<std::pair<ChessMove, double>> *best_path);

        void FreeTree(Node *node);

        bool IsRunning();

    private:
        const double C;
        std::atomic<bool> stop_,actioning;
        std::atomic<int> pause_cnt_;
        common::ThreadPool threadPool;
        Node* root_node_;
        ChessBoard root_board_;

        const int thread_num_;

        void LoopExpandTree();

//        void PrintNode(std::ostream &os, Node *node, ChessMove move, int deep);

        void getBestNode(Node *node, std::vector<std::pair<ChessMove, double>> *best_path);


    };
};

#endif //CCHESS_MCTSENGINE_H
