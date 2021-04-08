// C program for array implementation of queue
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*func)(void);

#define lf(p) (2*p+1)
#define rt(p) (2*p+2)

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

// Helper: less than operator for tasks (treated as pair<delay, prio>)
uint8_t compare(struct Task a, struct Task b)
{
    // min delay first
    if(a.delay < b.delay) return 1;
    // min priority on tie
    return (a.delay == b.delay) && (a.priority < b.priority);
}

// Max heap algorithm for dequeuing
void min_heap(struct Queue* q, int i)
{
    int small = i;
    int left = lf(i), right = rt(i);
    // get the minimum (left or right or this node)
    if(left <= q->currentSize && compare(q->tasksList[left], q->tasksList[small])) small = left;
    if(right <= q->currentSize && compare(q->tasksList[right], q->tasksList[small])) small = right;
    // if this node is the minimum then left, right and this subtree are heapified
    if (small == i) return;
    // else put the small on top and heapify the affected tree
    swap(&q->tasksList[small], &q->tasksList[i]);
    min_heap(q, small);
}

struct Task Dequeue(struct Queue* q)
{
//    if (isQueueFull(queue))
//        return default_task;
//
//    struct Task ret = default_task;
//    ret = queue->tasksList[0];
//    queue->tasksList[0] = queue->tasksList[queue->currentSize-1];
//    // Decrease queue size
//    queue->currentSize--;
//    // Put the last task (now at the top of the queue) in its correct place
//    bubbleSort(queue);
//    // return the desired task
//    return ret;

    struct Task ret = default_task;
    // Check for queue limits
    if (isQueueFull(q))
        return default_task;

//    if (q->currentSize == 0) {
//        printf("ERROR! - Queue is empty");
//        return ret;
//    }
    // Take task reference and replace its reference with the lasr task in the queue
    ret = q->tasksList[0];
    q->tasksList[0] = q->tasksList[q->currentSize-1];
    // Decrease queue size
    q->currentSize--;
    // Put the last task (now at the top of the queue) in its correct place
    min_heap(q, 0);
    // return the desired task
    return ret;
}

func func1(){
}

// Driver program to test above functions./
