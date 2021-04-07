// C program for array implementation of queue
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*func)(void);

// A structure to represent a queue
struct Task {
    int priority, delay;
    func task;
};

struct Queue {
    int currentSize, maxSize;
    struct Task* tasksList;
};
static const struct Task default_task = {INT_MAX, INT_MAX, NULL};

struct Queue* createQueue(int maxSize)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->maxSize = maxSize;
    queue->tasksList = (struct Task*)malloc(sizeof(struct Task));

    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isQueueFull(struct Queue* queue)
{
    return (queue->currentSize == queue->maxSize);
}

// Queue is empty when size is 0
int isQueuempty(struct Queue* queue)
{
    return (queue->currentSize == 0);
}
void swap(struct Task *task1, struct Task *task2)
{
    struct Task temp = *task1;
    *task1 = *task2;
    *task2 = temp;
}
void bubbleSort(struct Queue* queue)
{
    int i, j;
    for (i = 0; i < queue->currentSize-1; i++)
        for (j = 0; j < queue->currentSize-i-1; j++) if (queue->tasksList[j].priority > queue->tasksList[j+1].priority)
                swap(&queue->tasksList[j], &queue->tasksList[j+1]);
}
void Enqueue(struct Queue* queue, int priority, int delay, func task)
{
    if (isQueueFull(queue))
        return;

    struct Task t;
    t.task = task;
    t.priority = priority;
    t.delay = delay;

    queue->tasksList[queue->currentSize] = t;
    queue->currentSize++;


    bubbleSort(queue);

}
struct Task Dequeue(struct Queue* queue)
{
    if (isQueueFull(queue))
        return default_task;

    struct Task ret = default_task;
    ret = queue->tasksList[0];
    queue->tasksList[0] = queue->tasksList[queue->currentSize-1];
    // Decrease queue size
    queue->currentSize--;
    // Put the last task (now at the top of the queue) in its correct place
    bubbleSort(queue);
    // return the desired task
    return ret;

}
func func1(){

}

// Driver program to test above functions./
