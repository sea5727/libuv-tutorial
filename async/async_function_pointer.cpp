#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include <iostream>
#include <vector>
#include <thread>

class uvAsync{
public:
    uvAsync() = default;
    uv_async_t async;
    int error_code = 0;
    std::function<int()> callback = nullptr;
};

class innerClass{
public:
    innerClass() = default;
    int a = 100;
    std::string b = "b";
};

void timer_callback(
    uv_timer_t * handle) {
    std::cout << __func__ << std::this_thread::get_id() << std::endl;
}



typedef void (*uv_async_cb)(uv_async_t* handle);

void async_callback(uv_async_t* handle){

    auto uvasync = static_cast<uvAsync *>(handle->data);
    uvasync->callback();
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

        innerClass inner;
        int a = 1000;
        std::string b = "this is string";
        const char * c = "this is const char *";

        uvAsync * uvasync = new uvAsync;
        uvasync->async.data = uvasync;
        uvasync->callback = [=]{
            std::cout << "a:" << a << std::endl;
            std::cout << "b:" << b << std::endl;
            std::cout << "c:" << c << std::endl;
            std::cout << "inner.a:" << inner.a << std::endl;
            std::cout << "inner.b:" << inner.b << std::endl;
            return 0;
        };

        uv_async_init(loop, &uvasync->async, async_callback);
        uv_async_send(&uvasync->async);


    });

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    return 0;
}