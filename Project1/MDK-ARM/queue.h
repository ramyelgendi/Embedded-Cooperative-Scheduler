#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Definitions
typedef void (*func)(void);

// Structs
struct Task { int priority, delay; func task;};
struct Queue { int currentSize, maxSize; struct Task* tasksList; };
static const struct Task default_task = {INT_MAX, INT_MAX, NULL};

// Functions
struct Queue* createQueue(int);
int isQueueFull(struct Queue*);
int isQueuempty(struct Queue*);
void Enqueue(struct Queue*, int , int , func);
void heapify(struct Queue*, int);
struct Task Dequeue(struct Queue*);
int compare(struct Task, struct Task);

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
void bubbleSortForPriority(struct Queue* queue)
{
    for (int i = 0; i < queue->currentSize; i++)
        for (int j =i+1; j < queue->currentSize; j++)
            if (queue->tasksList[i].priority > queue->tasksList[j].priority)
            {
                struct Task temp = queue->tasksList[i];
                queue->tasksList[i] = queue->tasksList[j];
                queue->tasksList[j] = temp;
            }
}
void bubbleSortForDelay(struct Queue* queue)
{
    for (int i = 0; i < queue->currentSize; i++)
        for (int j =i+1; j < queue->currentSize; j++)
            if (queue->tasksList[i].delay > queue->tasksList[j].delay)
            {
                struct Task temp = queue->tasksList[i];
                queue->tasksList[i] = queue->tasksList[j];
                queue->tasksList[j] = temp;
            }
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
		if(delay==0)
			bubbleSortForPriority(queue);
		else
			bubbleSortForDelay(queue);

}

// Helper: less than operator for tasks (treated as pair<delay, prio>)
int compare(struct Task a, struct Task b)
{
    // min delay first
    if(a.delay < b.delay) return 1;
    // min priority on tie
    return (a.delay == b.delay) && (a.priority < b.priority);
}

// Max heap algorithm for dequeuing and making queue smaller
void heapify(struct Queue* q, int i)
{
    int smallest = i; // Initialize smalles as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
    // get the minimum (left or right or this node)
    if(l <= q->currentSize && compare(q->tasksList[l], q->tasksList[smallest])) smallest = l;
    if(r <= q->currentSize && compare(q->tasksList[r], q->tasksList[smallest])) smallest = r;
    // if this node is the minimum then left, right and this subtree are heapified
    if (smallest == i) return;
    // else put the small on top and heapify the affected tree

    struct Task temp = q->tasksList[smallest];
    q->tasksList[smallest] = q->tasksList[i];
    q->tasksList[i] = temp;

    heapify(q, smallest);
}
struct Task Dequeue(struct Queue* q)
{
    struct Task ret;

    if (isQueueFull(q))
        return default_task;

    // Take task reference and replace its reference with the lasr task in the queue
    ret = q->tasksList[q->currentSize-1];
    q->tasksList[q->currentSize-1] = q->tasksList[0]; // Get the one with highest priority (last in queue)
    // Decrease queue size
    q->currentSize--;
    // Put the last task (now at the top of the queue) in its correct place
    heapify(q, 0);
    // return the desired task
    return ret;
}