#include <pthread.h>
#define THREAD_POOL_SIZE 4
#define DNSRELAY_PORT 53
#define TASK_QUEUE_SIZE 4

//任务队列中的任务结构体
typedef struct task {
    void (*task_function)(void *arg);
    void *arg;//保存参数
} task_t;

//线程池结构体，入门版，还可以设置额外核心线程等等
typedef struct thread_pool {
    task_t *tasks;//任务队列，fifo
    int task_count;//队列里有多少个任务还没执行
    int task_head;//队头
    int task_tail;//队尾
    pthread_mutex_t mutex;//互斥锁，锁
    pthread_cond_t cond;//这个条件变量用于在任务队列为空时,让工作线程进入等待状态,直到有新任务加入。当有新任务加入时,条件变量会通知工作线程继续执行。
    int shutdown;//是否销毁
    pthread_t threads[THREAD_POOL_SIZE];//工作线程
} thread_pool_t;

void *thread_pool_worker(void *arg);

// 初始化线程池
thread_pool_t *thread_pool_create();

void thread_pool_add_task(thread_pool_t *pool, void (*task_function)(void *), void *arg);

void thread_pool_destroy(thread_pool_t *pool);