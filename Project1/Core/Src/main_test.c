//
// Created by Ramy ElGendi on 08/04/2021.
//
#include "queue.h"
static struct Queue* readyQueue;
static struct Queue* delayedQueue;
struct Task runningTask;
void func1(){
    printf("Task is running! \n");
}

void QueTask(func task,int priorty){
    if( priorty >=1 && priorty<=8 ) {
        Enqueue(readyQueue, priorty, 0, task);
    }

}

void Dispatch() {
    if(!isQueuempty(readyQueue)){
        runningTask = Dequeue(readyQueue);
        runningTask.task();
    }
}

void ReRunMe(int delay) {
    runningTask.delay=delay;
    if(delay==0)
        QueTask(runningTask.task, runningTask.priority);
    else
       Enqueue(delayedQueue,runningTask.priority,runningTask.delay,runningTask.task);

}

int main()
{
    readyQueue = createQueue(1000);
    delayedQueue = createQueue(1000);

    Enqueue(readyQueue, 5, 0, func1);
    Enqueue(readyQueue, 2, 0, func1);
    Enqueue(readyQueue, 1, 0, func1);
    Enqueue(readyQueue, 3, 0, func1);
    Enqueue(readyQueue, 6, 0, func1);


    int size = readyQueue->currentSize;
    for(int i=0;i<size && !isQueuempty(readyQueue);i++){
        struct Task temp = Dequeue(readyQueue);
        printf("Index %d, priority: %d\n",i,temp.priority);
        temp.task();
    }
    ReRunMe();
//    printf("%d '\n",Dequeue(queue)->priority);
//    printf("%d '\n",Dequeue(queue)->priority);
//    printf("%d '\n",Dequeue(queue)->priority);
//    printf("%d '\n",Dequeue(queue)->priority);

    return 0;
}
