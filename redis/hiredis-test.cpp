#include <stdio.h>
#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <uv.h>

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

#include <iostream>
#include <thread>
#include <vector>

#include "auAsync.hpp"

void 
connectCallback(
    const redisAsyncContext*, 
    int status){

    printf("connect Callback %d\n", status);
}


void 
disconnectCallback(
    const redisAsyncContext*, 
    int status){

    printf("disconnectCallback %d\n", status);
}

int main() {
 
    uv_loop_t* loop = uv_default_loop();

    redisAsyncContext *conn = redisAsyncConnect("127.0.0.1", 6379);
    if (conn->err) {
        return 1;
    }

    redisLibuvAttach(conn,loop);
    redisAsyncSetConnectCallback(conn, connectCallback);
    redisAsyncSetDisconnectCallback(conn,disconnectCallback);

    std::vector<std::thread> workers;
    workers.emplace_back([conn, loop]{

        char buffer[1024] = "";
        std::string cmd;
        while(1){
            fgets(buffer, sizeof(buffer), stdin);
            auAsync * auasync = new auAsync;
            auasync->NsQuery(conn, loop, [=]{
                auto res_cb = 
                    [](redisAsyncContext* conn,  void* r,  void* args){
                        redisReply *reply = (redisReply *)r;
                        if (reply == NULL) return;
                        printf("response:%s\n", reply->str);
                    };
                redisAsyncCommand(conn, res_cb, NULL, "SET MYKEYS 321321");
            });
            
        }
    });
    // redisAsyncCommand(c, getCallback, (char*)"end-1", "GET key");
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}