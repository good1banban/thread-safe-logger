#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "queue.h"
#include "logger.h"

MessageQueue queue; // 全局消息队列
pthread_mutex_t mutex; // 互斥锁
pthread_cond_t cond; // 条件变量

FILE *fp; // 日志文件指针

int running = 1; // 运行标志

void *producer(void *arg)
{
    int id = *(int*)arg;

    for(int i = 0; i < 10; i++)
    {   
        char msg[256];
        char timestamp[64];
        get_timestamp(timestamp, sizeof(timestamp)); // 获取当前时间戳
        sprintf(msg, 
                "[%s][%s][stread%d] hello %d",
                timestamp,
                level_to_string(INFO),  
                id,
                i); // 生成日志消息
        pthread_mutex_lock(&mutex); // 加锁
         
        queue_push(&queue, msg); // 将消息添加到队列

        pthread_cond_signal(&cond); // 发送条件变量信号，通知消费者有新消息
        pthread_mutex_unlock(&mutex); // 解锁

        sleep(1);// 模拟生产者工作

    }

    return NULL; // 线程退出
}



void *consumer(void *arg)
{
    char msg[256]; //   消息缓冲区

    while(running)
    {
        pthread_mutex_lock(&mutex); // 加锁

        if(queue.count == 0 && !running)
        {
            pthread_mutex_unlock(&mutex); // 解锁
            break; // 如果队列为空且生产者已停止，退出循环
        }

        queue_pop(&queue, msg); // 从队列中取出消息

        pthread_mutex_unlock(&mutex); // 解锁

        fprintf(fp, "%s\n", msg); // 将消息写入日志文件
        fflush(fp); // 刷新文件缓冲区

        printf("write :%s\n", msg); // 输出日志消息到控制台
    }
}

void start_logger()
{
    queue_init(&queue); // 初始化消息队列
    
    pthread_mutex_init(&mutex, NULL);// 初始化互斥锁
    pthread_cond_init(&cond, NULL); // 初始化条件变量

    fp = fopen("log.txt", "w"); // 打开日志文件
    if(fp == NULL)
    {
        perror("Failed to open log file");
        return;
    }
    pthread_t p1, p2, c1;    
    int id1 = 1, id2 = 2;
    
    pthread_create(&p1, NULL, producer, &id1); // 创建生产者线程1
    pthread_create(&p2, NULL, producer, &id2); // 创建生产者线程2
    pthread_create(&c1, NULL, consumer, NULL); // 创建消费者线程

    pthread_join(p1, NULL); // 等待生产者线程1结束
    pthread_join(p2, NULL); // 等待生产者线程2结束
    pthread_join(c1, NULL); // 等待消费者线程结束

    running = 0;
    pthread_cond_signal(&cond); // 发送条件变量信号，通知消费者线程退出
    pthread_join(c1, NULL); // 等待消费者线程退出

    fclose(fp); // 关闭日志文件

    sleep(2); // 等待消费者线程处理完所有消息

}

void get_timestamp(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info); // 获取当前时间戳
}

const char *level_to_string(LogLevel level)
{
    switch(level)
    {
        case INFO:
            return "INFO";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}