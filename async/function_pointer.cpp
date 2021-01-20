#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include <iostream>
#include <functional>
#include <vector>
#include <thread>

class uvWork{
public:
    uvWork() = default;
    uv_work_t work;
    int error_code = 0;
    std::function<int()> callback = nullptr;
};

class innserClass{
public:
    innserClass() = default;
    int a = 100;
    std::string b = "b";
};

void timer_callback(
    uv_timer_t * handle) {
    std::cout << __func__ << std::this_thread::get_id() << std::endl;
}
void queue_callback(uv_work_t* handle){
    std::cout << __func__ << std::this_thread::get_id() << std::endl;
    uvWork* uvwork = static_cast<uvWork*>(handle->data);
    std::cout << "callback?" << std::endl;
    int ret = uvwork->callback();
}
void queue_callback_after(uv_work_t* handle, int status){
    std::cout << __func__ << std::this_thread::get_id() << std::endl;
    uvWork* uvwork = static_cast<uvWork*>(handle->data);
    if(uvwork->error_code){
        std::cout << "Success "<< std::endl;
    } else {
        std::cout << "failure "<< std::endl;
    }
    delete uvwork;
}


int main() {

    std::cout << __func__ << std::this_thread::get_id() << std::endl;

    std::vector<std::thread> workers;

    
    uv_loop_t *loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    uv_timer_t * timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    uv_timer_start(timer, timer_callback, 0, 10000);

    workers.emplace_back([loop]{
        std::cout << "thread:" << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int a = 1000;
        std::string b = "this is string data";
        const char * c = "this is const char * data";

        innserClass inner;
        uvWork * uvwork = new uvWork;
        uvwork->callback = [=]{
            std::cout << "a:" << a << std::endl;
            std::cout << "b:" << b << std::endl;
            std::cout << "c:" << c << std::endl;
            std::cout << "inner.a:" << inner.a << std::endl;
            std::cout << "inner.b:" << inner.b << std::endl;
            return a;
        };
        uvwork->work.data = uvwork;
        
        std::cout << "uv_queue_work start?" << std::endl;
        uv_queue_work(loop, &uvwork->work , queue_callback, queue_callback_after);
        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    return 0;
}