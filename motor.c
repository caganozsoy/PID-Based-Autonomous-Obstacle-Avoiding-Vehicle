/*
 * File:   motor.c
 * Author: cagan
 *
 * Created on 12 Eylül 2026 Cumartesi, 23:03
 */


#include <xc.h>
#include "motor.h"

void motor_init(void){
    TRISD &= 0xF0;
    LATD &= 0xF0;
    
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    
    CCP1CON = 0x0C;
    CCP2CON = 0x0C;
    
    PR2 = 255;
    T2CON = 0x05; 

    
             
}

void robot_forward(void){
    IN1 = 1;
    IN2 = 0;
    IN3 = 1;
    IN4 = 0;
}

void adj_speed(uint16_t left_duty, uint16_t right_duty){
    CCPR1L = (uint8_t)(left_duty >> 2);
    CCP1CON = ((CCP1CON & 0xCF)) | ((left_duty & 0x0003)<<4);
    
    CCPR2L = (uint8_t)(right_duty >> 2);
    CCP2CON = ((CCP2CON & 0xCF)) | ((right_duty & 0x0003)<<4);
}