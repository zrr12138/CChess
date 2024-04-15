
#include <queue>
#include "common/task_thread_pool.h"
#include "common/thread_pool.h"
#include <cmath>
#include <list>
#include "ChessBoard.h"
#include "common/rw_lock.h"
#include "common/spinlock.h"
#include "common/uncopyable.h"
#include "gflags/gflags.h"
#include <unordered_map>
#ifndef CCHESS_MCTSENGINE_H
#define CCHESS_MCTSENGINE_H

DECLARE_int32(thread_num);
DECLARE_double(explore_c);

namespace CChess {

    struct SearchCtx;
    class MCTSEngine;

    struct Node {
        Node(bool is_red, Node *father, const ChessBoard &board, const ChessMove &move);
        ~Node();
        std::atomic<int64_t> n, red_win_count, black_win_count;
        std::atomic<int64_t> access_cnt;
        Node *father;
        bool inited;
        bool is_repeat;
        uint64_t node_hash;
        uint64_t board_hash;
        ChessMove move_;
        BoardResult end_;
        int end_deep_;
        common::SpinLock initLock;

        std::pair<ChessMove, Node*> *move_node_;
        int move_node_size_;

        bool is_red;

        void UpdateValue(BoardResult res);

        double GetWinRate(bool is_red_rate);

        std::pair<ChessMove, Node*> *GetBestMove();

        BoardResult ExpandTree(SearchCtx *ctx);
        BoardResult Simulation(SearchCtx *ctx);
        void Init(const ChessBoard &board);
        void SetRepeat();
        bool IsRepeat();


        void FilterByRule(const ChessBoard &board, std::vector<ChessMove> *moves);
    };

    struct SearchCtx {
        ChessBoard board;
    };

    class MCTSEngine: public common::Uncopyable{
        friend Node;
    public:
        static MCTSEngine& getInstance()
        {
            static MCTSEngine   instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

        ~MCTSEngine();

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

        const ChessBoard& GetChessBoard() const;

        std::string GetRootChooseStr() const;

    private:
        const double C;
        std::atomic<bool> stop_,actioning;
        std::atomic<int> pause_cnt_;
        common::ThreadPool *threadPool;
        Node* root_node_;
        ChessBoard root_board_;

        MCTSEngine():C(FLAGS_explore_c), thread_num_(FLAGS_thread_num), stop_(true),
                     actioning(false), threadPool(nullptr),
                     root_node_(nullptr) {}

        const int thread_num_;

        void LoopExpandTree();

//        void PrintNode(std::ostream &os, Node *node, ChessMove move, int deep);

        void getBestNode(Node *node, std::vector<std::pair<ChessMove, double>> *best_path);


    };

    class NodeHashManager : public common::Uncopyable{
    public:
        static NodeHashManager& getInstance()
        {
            static NodeHashManager instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
        static size_t GetNodeHash(size_t board_hash, bool is_red);
        void Add(size_t node_hash);
        void Remove(size_t node_hash);
        int Get(size_t node_hash);
        void Dump();
    private:
        common::SpinLock hash2cnt_lock_;
        std::unordered_map<size_t,int> hash2cnt;
    };
}

#endif //CCHESS_MCTSENGINE_H
