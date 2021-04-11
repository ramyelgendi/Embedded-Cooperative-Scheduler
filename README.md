# Embedded Cooperative Scheduler 
For this project, we developed a cooperative scheduler for Embedded systems by Mariam Farghaly(900170254), Hassan ELRashidy(900163312), Ramy ELGendi(900170269)
# Main functions
- QueTask: This function enqeues a task using its function pointer into the queue. It essentially calls the Enqueue function which handles the enqueuing of tasks according to the delay, if the delay is zero we enqueue in the ready queue else we enqueue in the delayed queue using delay and priority parameters.
- Dequeue: A function that to removes element from the readyQueue. It supports 8 priority levels using an if statement to check the validation of the user.
- DequeueDelay():A function that to removes element from the  delayedQueue.
- ReRunMe: This is a function that is used by the tasks to rerun periodically according to the time given in the delay parameters. If ReRunMe(0), the task will enqueue itself by calling it and remain in the ready queue.
- BubbleSortPriority: This function implements the simplest sorting algorithm that works by repeatedly swapping the adjacent elements if they are in wrong order according to the priority of the tasks.
- BubbleSortDelay: This is a similar function that implements the simplest sorting algorithm that works by repeatedly swapping the adjacent elements if they are in wrong order according to the delay value inserted by the user to the tasks.
- Dispatch(): This function gets called in the while loop, it removes the highest priority task from the queue and run it. It first checks whether the delayqueue is empty, then it calls the function Quetask(), enqueue the delayedQueue into the readyQueue. It also checks if the readyQueue isnot empty, performs the task and dequeue it. Also we set a flag to 1 when the delayedQueue and ReadyQueue is empty to check that everything is done.if 
- ReadyQueueIsEmpty(): A function that checks where the readyQueue is empty or not.
- DelayQueueIsEmpty():A function that checks where the delayQueue is empty or not.
- Init(): A function that initializes the scheduler data structures.

## Unit Tests
## Application 1: Ambient Temperature Monitor
In this application, we used the scheduler we created to create a small embedded application that initially asks the user to set a threshold temperature, then keeps on displaying every 30sec the temperature measured with a resolution of 0.25 degrees.

If the temperature increases the threshold, a message will display and an LED alarm will keep on flashing and the display of the temperature will stop.



- Demo (Speed x5)

![alt text](https://i.imgur.com/3Mza3Nr.gif)

- Extra Chips Used: DS3231

- Pins Used For I2C:
```bash
STM32     DS3231
D4     -> SDA
D5     -> SCL
5V     -> VCC
GND    -> GND
```
- Other Pins:
```bash
D0 -> Connected to send over UART1
D1 -> Connected to receive over UART1
```
- Connections

![alt text](https://i.imgur.com/nBvBalM.jpg)

## Application 2: Parking Sensor
The application uses the same schudler as the first one hence we have the same functions the only difference is the tasks. The application has two tasks; the first one is responsible for reading the distance between the sensor and the object. This happens by pulling the pull the TRIG pin HIGH then wait and pull the TRING pin LOW and transformed the distance measured into cm. The other task is responsible for toggling the buzzer based on the distance that it received from the first task. The frequency of the toggling increases when the distance increases to notify the user that the sensor got closer to an object. The toggling task has a higher priority since it needs to interrupt the reading task which is frequently working.
- Demo
https://user-images.githubusercontent.com/57822979/114319517-2bb60d80-9b12-11eb-8402-c67122c418d5.mp4

- Connections
![ParkingSensorConnection](https://user-images.githubusercontent.com/57822979/114319629-89e2f080-9b12-11eb-8f4b-408b3a470648.jpeg)

- Pins Configuration
![CubeMxParkingSensor](https://user-images.githubusercontent.com/57822979/114319630-8bacb400-9b12-11eb-8cdc-066cbd0e4b79.jpeg)
