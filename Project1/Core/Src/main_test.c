#include "queue.h"
static struct Queue* readyQueue;
static struct Queue* delayedQueue;
struct Task runningTask;

void Init(){
    readyQueue = createQueue(1000);
    delayedQueue = createQueue(1000);
}

void QueTask(func task,int priority){
    if(priority >= 1 && priority <= 8 )
    {
        Enqueue(readyQueue, priority, 0, task);
    }
}

void Dispatch() {
//    if(!isQueuempty(readyQueue)){
//        runningTask = Dequeue(readyQueue);
//        runningTask.task();
//    }
    int size = readyQueue->currentSize;
    for(int i=0;i<size && !isQueuempty(readyQueue);i++){
        struct Task runningTask = Dequeue(readyQueue);
        //printf("Index %d, priority: %d\n",i,temp.priority);
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
void TaskA(){
    printf("TaskA is running! \n");
    ReRunMe(0);
}
void TaskB(){
    printf("TaskB is running! \n");
    ReRunMe(0);
}
void TaskC(){
    printf("TaskC is running! \n");
    ReRunMe(0);
}

int main()
{
    Init();   // initialize the scheduler data structures
    QueTask(TaskA,8);
    QueTask(TaskB,2);
    QueTask(TaskC,5);
    Dispatch();
//    Enqueue(readyQueue, 5, 0, TaskA);
//    Enqueue(readyQueue, 2, 0, TaskA);
//    Enqueue(readyQueue, 1, 0, TaskA);
//    Enqueue(readyQueue, 3, 0, TaskA);
//    Enqueue(readyQueue, 6, 0, TaskA);
//
//    int size = readyQueue->currentSize;
//    for(int i=0;i<size && !isQueuempty(readyQueue);i++){
//        struct Task temp = Dequeue(readyQueue);
//        //printf("Index %d, priority: %d\n",i,temp.priority);
//        temp.task();
//    }
    //ReRunMe();
//    printf("%d '\n",Dequeue(queue)->priority);
//    printf("%d '\n",Dequeue(queue)->priority);
//    printf("%d '\n",Dequeue(queue)->priority);
//    printf("%d '\n",Dequeue(queue)->priority);

    return 0;
}
