#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <thread>
#include <iostream>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
    std::cout << "wait_for_a_while" << std::endl;
    uv_idle_stop(handle);
}
void cb(uv_timer_t* handle){
    std::cout << "timeout!!" << std::endl;
}

int main() {
    uv_loop_t *loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);


    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, cb, 0, 5000);

    while(1){
        
        int ret = uv_run(loop, UV_RUN_DEFAULT);
        printf("MyTest.ret:%d\n", ret);
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    

    uv_loop_close(loop);
    free(loop);

    return 0;
}