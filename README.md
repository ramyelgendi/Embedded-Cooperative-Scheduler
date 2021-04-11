# Embedded Cooperative Scheduler
 
 

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
