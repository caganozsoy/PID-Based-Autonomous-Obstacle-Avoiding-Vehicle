/*
 * File:   main.c
 * Author: cagan
 *
 * Created on September 11, 2026, 1:12 AM
 */


// PIC18F45K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "motor.h"
#include "sensor.h"
#include "pid.h"

#define _XTAL_FREQ 16000000

volatile uint16_t frnt_sens;
volatile uint16_t left_sens;
volatile uint16_t right_sens;

uint16_t base_speed = 900;

volatile float error = 0.0;
volatile float pid_value = 0.0;

void main(void) {
    OSCCON = 0x72;
    
    ANSELD = 0x00;
    ANSELC = 0x00;
    
    T0CON = 0x84; 
    TMR0H = 0xD8;
    TMR0L = 0xF0;
    
    INTCONbits.TMR0IF = 0;
    INTCONbits.PEIE = 1;  // derleyici patlak vermesin diye registerlar? önüne koydum no need for that!
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1;
   
    motor_init();
    sensor_init();
    
    robot_forward();
    
    while(1){
        
        frnt_sens = get_distance(FRONT_SENSOR);
        __delay_ms(10);
        left_sens = get_distance(LEFT_SENSOR);
        __delay_ms(10);
        right_sens = get_distance(RIGHT_SENSOR);
        __delay_ms(10);
        
        if(frnt_sens < 15 && frnt_sens > 1) {
            INTCONbits.TMR0IE = 0;
            LATDbits.LATD0 = 0; 
            LATDbits.LATD1 = 1;
            LATDbits.LATD2 = 0; 
            LATDbits.LATD3 = 1;
            
            adj_speed(800, 800);
            __delay_ms(600); 
             
             
            adj_speed(0, 0);
            __delay_ms(100);
            
            
            left_sens = get_distance(LEFT_SENSOR);
            __delay_ms(10);
            right_sens = get_distance(RIGHT_SENSOR);
            __delay_ms(10);
            
            PID_reset();
            error = (float)left_sens - (float)right_sens;

            robot_forward();
            INTCONbits.TMR0IE = 1;
            
        }else{
           float raw_error = (float)left_sens - (float)right_sens;
           if(raw_error > -3.0 && raw_error < 3.0){
               error = 0.0;
           }else{
               error = raw_error;
           }
        }

    }
    
    return;
}

void __interrupt(high_priority) isr(void){
    if (INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0H = 0xD8;
        TMR0L = 0xF0;
    
        pid_value = PIDcontrol(error, 0.02);
        
        int16_t left_pwm = (int16_t)(base_speed + pid_value);
        int16_t right_pwm = (int16_t)(base_speed - pid_value);
        
        if(left_pwm > 1023){
            left_pwm = 1023;
        }
        if(left_pwm < 600){
            left_pwm = 600;
        }
        if(right_pwm > 1023){
            right_pwm = 1023;
        }
        if(right_pwm < 600){
            right_pwm = 600;
        }
       
        adj_speed((left_pwm), (right_pwm));
    }
}

