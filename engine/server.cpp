//
// Created by zhengran on 2024/3/17.
//
#include "crow.h"

int main(int argc, char *argv[])
{
    crow::SimpleApp app;
    int a=5;

    CROW_ROUTE(app, "/")([&a](){
        return "Hello world"+std::to_string(a++);
    });

    app.concurrency(1).port(18080).run();
}