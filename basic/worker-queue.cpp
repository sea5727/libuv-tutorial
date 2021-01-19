#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>
#include <iostream>
#include <thread>
#include <iostream>

#define FIB_UNTIL 25

uv_loop_t * loop;

long fib_(long t) {
  if (t == 0 || t == 1)
    return 1;
  else
    return fib_(t-1) + fib_(t-2);
}

void fib(uv_work_t *req) {
  
  int n = *(int *) req->data;
  std::cout << "fib n : " << n << std::endl;
  if (random() % 2){
      std::cout << n << "]sleep1 :" << std::this_thread::get_id() << std::endl;;
      sleep(1);
  }
  else{
      std::cout << n << "]sleep3 :" << std::this_thread::get_id() << std::endl;;
      sleep(3);
  }
    
  long fib = fib_(n);
  fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

void after_fib(uv_work_t *req, int status) {
  fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
}

int main() {

  // "UV_THREADPOOL_SIZE=%lu"
  loop = uv_default_loop();

  int data[FIB_UNTIL];
  uv_work_t req[FIB_UNTIL];
  int i;
  for (i=0; i<FIB_UNTIL; i++) {
    data[i] = i;
    req[i].data = (void *) &data[i];
    std::cout << "uv_queue_work call i:" << i << std::endl;
    uv_queue_work(loop, &req[i], fib, after_fib);
  }
  return uv_run(loop, UV_RUN_DEFAULT);
}