//
// Created by zhengran on 2024/3/17.
//
#include "crow.h"
#include "ChessBoard.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "errorcode.h"
using namespace CChess;
int main(int argc, char *argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging("CChess_server");
    FLAGS_log_dir = ".";
    FLAGS_v = 2;
    ChessBoard board;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([&](){
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

    //board api
    CROW_ROUTE(app, "/init_board")([&](const crow::request& req){
        ChessBoard board;
        board.initBoard();
        auto r=crow::json::load(board.ToString());
        return crow::json::wvalue({{"error",OK},{"board",r}});
    });
    CROW_ROUTE(app, "/board_move")([&](const crow::request& req){
        auto req_json = crow::json::load(req.body);
        if(!req_json){
            return crow::json::wvalue({{"error",BAD_ARGUMENT}});
        }
        if(!req_json.has("move")||!req_json.has("board")){
            return crow::json::wvalue({{"error",BAD_ARGUMENT}});
        }
        ChessMove move;
        move.ParseFromString(req_json["move"].s());
        ChessBoard board;
        board.ParseFromString(req_json["board"].s());
        if(board.Move(move)){
            crow::json::wvalue({{"error",OK},{"board",crow::json::load(board.ToString())}});
        }
        return crow::json::wvalue({{"error",MOVE_FAILED}});
    });
    app.concurrency(1).port(18080).run();

}