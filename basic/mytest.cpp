#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
    counter++;

    if (counter >= 10e6)
        uv_idle_stop(handle);
}

int main() {
    uv_loop_t *loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);


    uv_idle_t idler;
    uv_idle_init(loop, &idler);
    uv_idle_start(&idler, wait_for_a_while);

    printf("MyTest.\n");
    uv_run(loop, UV_RUN_ONCE);

    uv_loop_close(loop);
    free(loop);

    return 0;
}