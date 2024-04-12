//
// Created by zhengran on 2024/3/17.
//

#include "crow.h"
#include "ChessBoard.h"
#include "MCTSEngine.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "errorcode.h"

using namespace CChess;

DEFINE_int32(thread_num, 8, "");

int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging("CChess_server");
    FLAGS_log_dir = ".";
    FLAGS_v = 2;
    ChessBoard board;
    MCTSEngine engine(FLAGS_thread_num);
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([&]() {
        return "https://github.com/zrr12138/CChess";
    });

//    CROW_ROUTE(app, "/action")([&](const crow::request& req){
//        char *start_x_s=req.url_params.get("start_x");
//        char *start_y_s=req.url_params.get("start_y");
//        char *end_x_s=req.url_params.get("end_x");
//        char *end_y_s=req.url_params.get("end_y");
//        if(start_x_s==NULL||start_y_s==NULL||end_x_s==NULL||end_y_s==NULL){
//            return crow::json::wvalue({{"error",BAD_REQUEST}});
//        }
//        LOG(INFO)<<"/action "<<"start_x:"<<start_x_s<<" start_y:"<<start_y_s<<" end_x:"<<end_x_s<<" end_y:"<<end_y_s;
//        int start_x=atoi(start_x_s);
//        int start_y=atoi(start_y_s);
//        int end_x=atoi(end_x_s);
//        int end_y=atoi(end_y_s);
//        ChessMove move(start_x,start_y,end_x,end_y);
//        board.Move(move);
//        return crow::response(200);
//    });

    //engine api
    CROW_ROUTE(app, "/start_search").methods(crow::HTTPMethod::Post)([&](const crow::request &req) {
        auto req_json = crow::json::load(req.body);
        LOG(INFO) << "/start_search " << req.body;
        if (!req_json) {
            return crow::json::wvalue({{"error", BAD_ARGUMENT}});
        }
        if (!req_json.has("board") || !req_json.has("red_first")) {
            return crow::json::wvalue({{"error", ARGUMENT_NOT_ENOUGH}});
        }
        bool red_first = req_json["red_first"].b();
        ChessBoard board;
        board.ParseFromString(req_json["board"].s());
        if (engine.StartSearch(board, red_first)) {
            return crow::json::wvalue({{"error", OK}});
        } else {
            return crow::json::wvalue({{"error", START_SEARCH_FAILED}});
        }
    });

    CROW_ROUTE(app, "/engine_action")([&](const crow::request &req) {
        if (!engine.IsRunning()) {
            return crow::json::wvalue({{"error", ENGINE_IS_NOT_RUNNING}});
        }
        auto move = engine.GetResult();
        assert(engine.Action(move));
        assert(board.Move(move));
        return crow::json::wvalue({{"error", OK}});
    });

    CROW_ROUTE(app, "/engine_stop")([&](const crow::request &req) {
        assert(engine.Stop());
        return crow::json::wvalue({{"error", OK}});
    });

    //board api
    CROW_ROUTE(app, "/init_board")([&](const crow::request &req) {
        ChessBoard board;
        board.initBoard();
        auto r = crow::json::load(board.ToString());
        return crow::json::wvalue({{"error", OK},
                                   {"board", r}});
    });
    CROW_ROUTE(app, "/board_move").methods(crow::HTTPMethod::Post)([&](const crow::request &req) {
        auto req_json = crow::json::load(req.body);
        LOG(INFO) << "/board_move " << req.body;
        if (!req_json) {
            return crow::json::wvalue({{"error", BAD_ARGUMENT}});
        }
        if (!req_json.has("move") || !req_json.has("board")) {
            return crow::json::wvalue({{"error", ARGUMENT_NOT_ENOUGH}});
        }
        ChessMove move;
        move.ParseFromString(req_json["move"].s());
        ChessBoard board;
        board.ParseFromString(req_json["board"].s());

        if(engine.IsRunning()){
            if(engine.Action(move))
            {
                assert(board.Move(move));
                return crow::json::wvalue({{"error", OK},
                                           {"board", crow::json::load(board.ToString())}});
            }
        }
        else{
            if(board.Move(move)){
                return crow::json::wvalue({{"error", OK},
                                           {"board", crow::json::load(board.ToString())}});
            }
        }
        return crow::json::wvalue({{"error", MOVE_FAILED}});
    });
    app.concurrency(1).port(12138).run();

}