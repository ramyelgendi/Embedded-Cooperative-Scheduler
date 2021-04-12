# Embedded Cooperative Scheduler 
- For this project, we developed a cooperative scheduler for Embedded systems using STM32CubeMX and Keil μVision running it on the Nucleo-32 board.

## Contributors
- Mariam Farghaly  (900170254)
- Hassan ElRashidy (900163312)
- Ramy ElGendi     (900170269)

# Main functions
- QueTask: This function enqueues a task into the queue. It essentially calls the Enqueue function which handles the enqueuing of tasks according to the delay, if the delay is zero we enqueue in the ready queue else we enqueue in the delayed queue using delay and priority parameters.
- Dequeue: 
 A function that to removes element from the readyQueue. It supports 8 priority levels using an if statement to check the validation of the user.
- DequeueDelay(): A function that to removes element from the  delayedQueue.
- ReRunMe:This is a function that is used by the tasks to rerun periodically according to the time given in the delay parameters. If ReRunMe(0), the task will enqueue itself by calling it and remain in the ready queue.
- BubbleSortPriority:This function implements the simplest sorting algorithm that works by repeatedly swapping the adjacent elements if they are in wrong order according to the priority of the tasks.
- BubbleSortDelay: 
This is a similar function that implements the simplest sorting algorithm that works by repeatedly swapping the adjacent elements if they are in wrong order according to the delay value inserted by the user to the tasks.
- Dispatch():This function gets called in the while loop, it removes the highest priority task from the queue and run it. It first checks whether the delayqueue is empty, then it calls the function Quetask(), enqueue the delayedQueue into the readyQueue. It also checks if the readyQueue isnot empty, performs the task and dequeue it. Also we set a flag to 1 when the delayedQueue and ReadyQueue is empty to check that everything is done.
- ReadyQueueIsEmpty():A function that checks where the readyQueue is empty or not.
- DelayQueueIsEmpty():A function that checks where the delayQueue is empty or not.
- Init(): A function that initializes the scheduler data structures.
- SysTick Handler: We implemented after each tick, we decrement all delay values and checkif any task should be dequeued and added to the readyQueue achieving 1 tick = 50 msec.
# Data structures
- Struct Task: This struct has 3 fields: The task's priority, delay and a pointer to the name of the function. 
- Struct Queue: This struct has 3 fields: the current size, maximum size of queue, and a task struct pointer pointing to the tasks arrat of what the queue contains.
- Typedef func: void function pointer.

# Unit Tests
## Scheduler Demo 1
Demo consists of 5 tasks. They do not use ReRunMe function for testing the readyQueue. Tasks are being defined as follows:

![WhatsApp Image 2021-04-12 at 12 00 03 AM](https://user-images.githubusercontent.com/68485300/114322906-0382da80-9b23-11eb-9da1-a86086a5059a.jpeg)
### Expected Output
![Screen Shot 2021-04-11 at 11 45 23 PM](https://user-images.githubusercontent.com/68485300/114322416-6b83f180-9b20-11eb-923c-d211a6cc8703.png)
###  Output
![TestCase3](https://user-images.githubusercontent.com/68485300/114321920-cb2ccd80-9b1d-11eb-9ac5-ab83b5119ebd.jpeg)

## Scheduler Demo 2
Demo consists of 1 task, having ReRunMe(3), meaning that it has delay of (3). Tasks defined as:

![WhatsApp Image 2021-04-12 at 12 11 35 AM](https://user-images.githubusercontent.com/68485300/114323122-15b14880-9b24-11eb-8c2f-e9b6518805c1.jpeg)
### Output
![TestCase](https://user-images.githubusercontent.com/68485300/114321802-fd89fb00-9b1c-11eb-932e-9b5fa8bf25a3.jpeg)
## Scheduler Demo 3
Demo consists of two tasks, having different priorities and different delays. Tasks are defined as the following:

![WhatsApp Image 2021-04-12 at 12 12 21 AM](https://user-images.githubusercontent.com/68485300/114323137-31b4ea00-9b24-11eb-9b8d-86fb8da58f04.jpeg)
### Output
![TestCase1](https://user-images.githubusercontent.com/68485300/114321805-ffec5500-9b1c-11eb-965f-77dd1d7ea874.jpeg)

## Scheduler Demo 4
Demo consists of 2 tasks, having same delay and same priority. We defined the tasks as follows:

![Third](https://user-images.githubusercontent.com/68485300/114322923-1dbcb880-9b23-11eb-81e4-0f6b58084cbd.jpeg)
### Output
![TestCase2](https://user-images.githubusercontent.com/68485300/114321807-01b61880-9b1d-11eb-95a7-0660bcb91a3a.jpeg)

## Scheduler Demo 5
Demo consists of 4 tasks, with different delays and priorities, showing the full functionality of the scheduler.
### Expected Output
![Last](https://user-images.githubusercontent.com/68485300/114322928-2a411100-9b23-11eb-970a-104de99abb3f.jpeg)
![Screen Shot 2021-04-11 at 11 43 35 PM](https://user-images.githubusercontent.com/68485300/114322419-6f177880-9b20-11eb-90ea-9cc08642c242.png)
### Output
![image](https://user-images.githubusercontent.com/68485300/114321832-3629d480-9b1d-11eb-9686-8bb41185f130.png)



## Application 1: Ambient Temperature Monitor
In this application, we used the scheduler we created to create a small embedded application that initially asks the user to set a threshold temperature, then keeps on displaying every 30sec the temperature measured with a resolution of 0.25 degrees. We used the hexaToAscii function, intialized the temperature registers. Used  ```bash HAL_I2C_Master_Transmit ``` to send whole number register address 11h to read from and the fraction portion register address 12h to read from. Checking on the four cases of the fraction portion ('0'--> .00, '64'-->0.25, '128'-->0.50,'192'-->0.75). Then, transmitting that to the uart2 to see the output on TeraTerm.  
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
The application uses the same scheduler as from the beginning, hence we have the same functions the only difference is the tasks . The application has two tasks; the first one is responsible for reading the distance between the sensor and the object. This happens by pulling the pull the TRIG pin HIGH then wait and pull the TRING pin LOW and transformed the distance measured into cm. The other task is responsible for toggling the buzzer based on the distance that it received from the first task. The frequency of the toggling increases when the distance increases to notify the user that the sensor got closer to an object. The toggling task has a higher priority since it needs to interrupt the reading task which is frequently working.
- Demo
https://user-images.githubusercontent.com/57822979/114319517-2bb60d80-9b12-11eb-8402-c67122c418d5.mp4

- Connections
![ParkingSensorConnection](https://user-images.githubusercontent.com/57822979/114319629-89e2f080-9b12-11eb-8f4b-408b3a470648.jpeg)

- Pins Configuration on STM32CubeMX
![CubeMxParkingSensor](https://user-images.githubusercontent.com/57822979/114319630-8bacb400-9b12-11eb-8cdc-066cbd0e4b79.jpeg)
