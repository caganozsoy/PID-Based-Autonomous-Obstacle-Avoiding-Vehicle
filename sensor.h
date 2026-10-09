#ifndef SENSOR_H
#define SENSOR_H



#include <xc.h>//
#include <stdint.h>
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

#define TRIG_FRONT LATBbits.LATB0
#define ECHO_FRONT PORTBbits.RB1

#define TRIG_LEFT LATBbits.LATB2
#define ECHO_LEFT PORTBbits.RB3

#define TRIG_RIGHT LATBbits.LATB4
#define ECHO_RIGHT PORTBbits.RB5

typedef enum{
    FRONT_SENSOR, LEFT_SENSOR, RIGHT_SENSOR
}sensor_t;

void sensor_init(void);

uint16_t get_distance(sensor_t sensor);

#endif

