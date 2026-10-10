# PID-Based-Autonomous-Obstacle-Avoiding-Vehicle
A PID-based autonomous vehicle. Involves MPLAB codes that powers the microcontroller PIC18F45K22.

MPLAB IDE and XC8 compiler is used for integrating our PIC controller. Since I had a microcontroller lesson coded KOM2732, in advance to that I convinced myself to use a PIC controller to power this autonomous vehicle.
There are 3 .c files, 3 header files, 1 main.c file for our main code and a MakeFile file. 

1. motor.c and motor.h:
   Involves 3 functions.
   1. motor_init(): initializing the motor, configuring the bits that enables or disables the DC motor.
   2. robot_forward(): motion of the vehicle is mainly coded in here.
   3. adj_speed(left and right PWM): adjusting the PWM duty cycles for speed of the motor.
2. pid.c and pid.h
   Involves gains gathered from Simulink Model. A reset and a control function has been writen.
   1. PID_reset(): resetting PID for backward movement since the cumulative error prevents the car moving backwards. So a reset function is needed for calculating the error.
   2. PIDcontrol(error, dt): equation of the PID is formed. Containing a feedback mechanism.
3. sensor.c and sensor.h
   Initialization and information gathering based on movement of vehicle is configured here.
   1. sensor_init(): initialization of the sensor by configuring the bits where each sensor is connected to the PIC
   2. get_distance(sensor): calculating the distance between the chosen sensor and the object.
4. main.c
   main.c is the main code that all header files are gathered for the final speculation of the vehicle. A 20ms interrupt mechanism is used for constantly delivering power to the motors. Furthermore, an interrupt
   mechanism in provoked when the timer configuration bit is disabled for a backward motion. So the vehicle autonomously controls the motion it is supposed to do. 
