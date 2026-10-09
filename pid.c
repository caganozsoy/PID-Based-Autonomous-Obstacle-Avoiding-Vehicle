#include <xc.h>
#include "pid.h"


const float Kp = 1.9196;
const float Ki = 0.0569;
const float Kd = 0.0211;

static float prev_error = 0;
static float integral = 0;

void PID_reset(){
    integral = 0;
    prev_error = 0;
}
float PIDcontrol(float error, float dt){
    
    integral += error * dt;
    
    float P = Kp * error;
    float I = Ki * integral;
    float D = Kd * ((error - prev_error) / dt);
    
    float PID = P + I + D;
    
    if((PID > 1023 && error > 0)){
        PID = 1023;
        integral -= error * dt;
    }
    else if(PID < -1023 && error < 0){
        PID = -1023;
        integral -= error * dt;
    }
    
    prev_error = error;
    
    return(PID);
}

