//
// Created by nocnname on 2024/7/1.
//
#include <stdlib.h>
#include "Thread_pool.h"

void *thread_pool_worker(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;

    while (1) {
        pthread_mutex_lock(&pool->mutex);

        while (pool->task_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutex);
            pthread_exit(NULL);
        }

        task_t task = pool->tasks[pool->task_head];
        pool->task_head = (pool->task_head + 1) % TASK_QUEUE_SIZE;
        pool->task_count--;

        pthread_mutex_unlock(&pool->mutex);//取出任务，但是还未执行就可以释放锁了，因为不会冲突了，和想的一样

        task.task_function(task.arg);//各线程并发执行任务
    }

    return NULL;
}

// 初始化线程池
thread_pool_t *thread_pool_create() {
    thread_pool_t *pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    pool->tasks = (task_t *)malloc(sizeof(task_t) * TASK_QUEUE_SIZE);//创建任务队列，队列长暂时先设为线程池大小
    pool->task_count = 0;
    pool->task_head = 0;
    pool->task_tail = 0;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->shutdown = 0;

    //这里不仅创建了各个线程，并且让每个线程都开启，并进入thread_pool_worker函数并无限循环，开始工作
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&pool->threads[i], NULL, thread_pool_worker, pool);
    }

    return pool;
}

void thread_pool_add_task(thread_pool_t *pool, void (*task_function)(void *), void *arg) {
    pthread_mutex_lock(&pool->mutex);//尝试获取锁

    while (pool->task_count == TASK_QUEUE_SIZE) {
        pthread_cond_wait(&pool->cond, &pool->mutex);
    }

    pool->tasks[pool->task_tail].task_function = task_function;
    pool->tasks[pool->task_tail].arg = arg;
    pool->task_tail = (pool->task_tail + 1) % TASK_QUEUE_SIZE;//队列尾部添加任务
    pool->task_count++;

    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);//释放锁
}

//利用shutdown广播销毁线程池
void thread_pool_destroy(thread_pool_t *pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    free(pool->tasks);
    free(pool);
}
