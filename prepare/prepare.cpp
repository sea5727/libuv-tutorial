#include <stdio.h>
#include <uv.h>
#include <vector>
#include <thread>
int64_t counter = 0;

void idle_cb(uv_idle_t *handle) {
    printf("Idle callback\n");
    counter++;

    if (counter >= 5) {
        uv_stop(uv_default_loop());
        printf("uv_stop() called\n");
    }
}

void prep_cb(uv_prepare_t *handle) {
    printf("Prep callback\n");
}

void timer_cb(uv_timer_t * handle){
    printf("timer_cb callback\n");
}

int main() {

    uv_prepare_t prep;




    uv_timer_t timer;
    uv_timer_init(uv_default_loop(), &timer);
    uv_timer_start(&timer, timer_cb, 0, 5000);

    uv_prepare_init(uv_default_loop(), &prep);
    uv_prepare_start(&prep, prep_cb);

    std::vector<std::thread> workers;
    workers.emplace_back([]{

        std::this_thread::sleep_for(std::chrono::seconds(1));

        uv_idle_t idler;
        uv_idle_init(uv_default_loop(), &idler);
        uv_idle_start(&idler, idle_cb);

        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}