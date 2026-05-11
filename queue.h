#ifndef QUEUE_H
#define QUEUE_H

#define MAX_MSG_LEN 256 // 消息最大长度
#define QUEUE_SIZE 100 // 队列最大容量
typedef struct {
    char messages[QUEUE_SIZE][MAX_MSG_LEN]; // 消息存储数组
    int front; // 队列头索引
    int rear; // 队列尾索引
    int count; // 当前队列中的消息数量
} MessageQueue;

void queue_init(MessageQueue *queue);// 初始化队列
int queue_push(MessageQueue *queue, const char *message); // 向队列中添加消息
int queue_pop(MessageQueue *queue, char *buffer); // 从队列中取出

#endif

