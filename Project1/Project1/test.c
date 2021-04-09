#include  <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Useful defines and typedefs
#define lf(p) (2*p+1)
#define rt(p) (2*p+2)
#define p(c) ((c-1)/2)
#define min(a,b) (a<b?a:b)
typedef void(*fptr)(void);

// Task object
struct task {
	fptr f;
	uint16_t prio;
	uint16_t delay;
};
static const struct task default_task = { NULL, 65535, 65535 };
// Task queue
struct task_queue {
	int cur_sz;
	int max_sz;
	struct task * tasks;
};

// Function declarations
void q_init(struct task_queue * q, uint16_t size);
void enqueue(struct task_queue* q, fptr f, uint16_t prio, uint16_t delay);
void _enqueue(struct task_queue* q, struct task new_task);
void min_heap(struct task_queue* q, int i);
void swap(struct task *task1, struct task *task2);
struct task dequeue(struct task_queue* q);
void decrement_all(struct task_queue* q, uint16_t cnt);
void push_all_ready(struct task_queue* delayed_q, struct task_queue* main_q);
uint8_t compare(struct task a, struct task b);

// Helper: less than operator for tasks (treated as pair<delay, prio>)
uint8_t compare(struct task a, struct task b)
{
	// min delay first
	if (a.delay < b.delay) return 1;
	// min priority on tie
	return (a.delay == b.delay) && (a.prio < b.prio);
}

// Helper: Swap task references
void swap(struct task *task1, struct task *task2)
{
	struct task temp = *task1;
	*task1 = *task2;
	*task2 = temp;
}

// Task queue constructor
void q_init(struct task_queue * q, uint16_t size)
{
	q->cur_sz = 0;
	q->max_sz = size;
	q->tasks = (struct task*)malloc(size * sizeof(struct task));
	for (uint16_t i = 0; i < q->max_sz; i++) q->tasks[i] = default_task;
}

// Enqueue function
void enqueue(struct task_queue* q, fptr f, uint16_t prio, uint16_t delay)
{
	// Create new task and call the private enqueue function
	volatile struct task new_task = { f, prio, delay };
	_enqueue(q, new_task);
}

// Enqueue function
void _enqueue(struct task_queue* q, struct task new_task)
{
	// Check for queue limits
	if (q->cur_sz == q->max_sz - 1) {
		printf("ERROR! - Queue is full");
		return;
	}
	// put at end of the queue
	q->tasks[q->cur_sz++] = new_task;

	// Go up the tree till it can't overthrow its parent
	int i = q->cur_sz - 1;
	while (i != 0 && compare(q->tasks[i], q->tasks[p(i)])) {
		swap(&q->tasks[p(i)], &q->tasks[i]);
		i = p(i);
	}
}

// Max heap algorithm for dequeuing
void min_heap(struct task_queue* q, int i)
{
	int small = i;
	int left = lf(i), right = rt(i);
	// get the minimum (left or right or this node)
	if (left <= q->cur_sz && compare(q->tasks[left], q->tasks[small])) small = left;
	if (right <= q->cur_sz && compare(q->tasks[right], q->tasks[small])) small = right;
	// if this node is the minimum then left, right and this subtree are heapified
	if (small == i) return;
	// else put the small on top and heapify the affected tree
	swap(&q->tasks[small], &q->tasks[i]);
	min_heap(q, small);
}

// Dequeue function
struct task dequeue(struct task_queue* q)
{
	struct task ret = default_task;
	// Check for queue limits
	if (q->cur_sz == 0) {
		printf("ERROR! - Queue is empty");
		return ret;
	}
	// Take task reference and replace its reference with the lasr task in the queue
	ret = q->tasks[0];
	q->tasks[0] = q->tasks[q->cur_sz - 1];
	// Decrease queue size
	q->cur_sz--;
	// Put the last task (now at the top of the queue) in its correct place
	min_heap(q, 0);
	// return the desired task
	return ret;
}

// Decrements all the delays in a certain queue (used with the delayed queue)
// Guarded with 'cur_sz' --> no need for external guard
void decrement_all(struct task_queue* q, uint16_t cnt)
{
	for (uint8_t i = 0; i < q->cur_sz; i++)
		q->tasks[i].delay -= min(cnt, q->tasks[i].delay);
}

// Pushes all ready tasks from the delayed queue to the main queue
// Guarded with 'cur_sz' --> no need for external guard
void push_all_ready(struct task_queue* delayed_q, struct task_queue* main_q)
{
	while (delayed_q->cur_sz > 0 && delayed_q->tasks[0].delay == 0)
		_enqueue(main_q, dequeue(delayed_q));
}

fptr fptr1()
{

}
// Driver program to test above functions./
int main()
{
	struct task_queue *queue;
	q_init(queue, 1000);
	enqueue(queue, fptr1, 4, 0);
	enqueue(queue, fptr1, 3, 0);
	enqueue(queue, fptr1, 2, 0);
	enqueue(queue, fptr1, 5, 0);
	enqueue(queue, fptr1, 1, 0);

	for (int i = 0; i<queue->cur_sz; i++) {
		printf("%d\n", queue->tasks[i].prio);
	}
	//    printf("%d '\n",Dequeue(queue)->priority);
	//    printf("%d '\n",Dequeue(queue)->priority);
	//    printf("%d '\n",Dequeue(queue)->priority);
	//    printf("%d '\n",Dequeue(queue)->priority);

	return 0;
}
