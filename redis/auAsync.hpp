#pragma once

#include <uv.h>
#include <string>
#include <functional>

class auAsync{
public:
    auAsync() 
        : error_code{0}
        , error_message{""} {
    }
    void
    NsQuery(redisAsyncContext * conn, uv_loop_t * loop, std::function<void()> f){
        handle.data = this;
        work_func = f;
        
        uv_async_init(loop, &handle, auAsync::callback);
        uv_async_send(&handle);
    }

    static
    void
    callback(uv_async_t * handle) {
        auAsync * auasync = static_cast<auAsync *>(handle->data);
        auasync->work_func();
    }
    

    uv_async_t handle;
    std::function<void()> work_func;
    int error_code;
    std::string error_message = 0;

};