# Garbage Management Using Line Follower Bot

The primary goal of the project is to implement a automated garbage collection. 
This project is done as part of [IT825 Designing of Internet of Things](https://infotech.nitk.ac.in/course/it825) taught at National Institute of Technology Karnataka.

## Components used:
- Arduino Yun
- NodeMCU
- Ultrasonic Sensor
- Servo Motor
- Line Follower Bot

## Design
### Line Follower
Arduino Yun is mounted on the bot with a garbage collector which has a shaft built with it.
This shaft is lifted using the servo motor to dump waste at dumping yard.

### Dustbin
Two dustbins were used in this project each made out of cardboard. Inside each dustbin there is a ultrasonic sensor and a servo motor.
Ultrasonic sensor is used to update the amount of garbage in the bin. The servo is used to open the lid and the garbage fell down on the collector.
The dustbins were mounted upside down so that the openings were at the bottom.

### Line
```
                    Bin 2             Bin 1
----------------------|-----------------|------------------
|          <-                 <-             <-           |
|                                                         |
|          ->         Direction of Bot       ->           |
-----------------------------|-----------------------------
                         Dumping Yard
```

Authors : [Sampat Ghosh](https://github.com/sampatghosh), [Shaikh Sahil Ahmed](https://github.com/ShaikhSahilAhmed), Rohit Meshram
