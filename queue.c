#include <string.h>
#include "queue.h"

void queue_init(MessageQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
}

int queue_push(MessageQueue *queue, const char *message) {

    if(queue->count >= QUEUE_SIZE)
    {
        queue->front = (queue->front + 1) % QUEUE_SIZE; // 队列满时覆盖最旧的消息
        queue->count--; // 减少消息数量
    }

    strcpy(queue->messages[queue->rear], message); // 将消息复制到队列尾部
    queue->rear = (queue->rear + 1) % QUEUE_SIZE; // 更新队列尾索引
    queue->count++; // 增加消息数量
    return 0; // 成功添加消息
}

int queue_pop(MessageQueue *queue, char *msg) {
    if(queue->count <= 0)
    {
        return -1; // 队列为空
    }

    strcpy(msg, queue->messages[queue->front]); // 从队列头部复制消息到缓冲区
    queue->front = (queue->front + 1) % QUEUE_SIZE; //  更新队列头索引
    queue->count--; // 减少消息数量
    return 0; // 成功取出消息
}