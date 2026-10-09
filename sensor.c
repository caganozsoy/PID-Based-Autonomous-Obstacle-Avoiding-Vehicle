#include <xc.h>
#define _XTAL_FREQ 16000000
#include "sensor.h"

void sensor_init(void){
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 1;

    ANSELB = 0x00;
    T1CON = 0x20;
    
}

uint16_t get_distance(sensor_t sensor){
    uint16_t timeout;
    
    switch(sensor){
        case FRONT_SENSOR:
            TRIG_FRONT = 1;
            __delay_us(10);
            TRIG_FRONT = 0;

            timeout = 10000;
            while(ECHO_FRONT == 0 && --timeout > 0);
            if(timeout == 0) return 400; // Sensör yan?t vermezse güvenli mesafe dön

            TMR1 = 0;
            T1CONbits.TMR1ON = 1;

            // Yank? süresini ölç (Timeout Korumal?)
            timeout = 10000;
            while(ECHO_FRONT == 1 && --timeout > 0);
            T1CONbits.TMR1ON = 0;
            break;

        case LEFT_SENSOR:
            TRIG_LEFT = 1;
            __delay_us(10);
            TRIG_LEFT = 0;

            timeout = 10000;
            while(ECHO_LEFT == 0 && --timeout > 0);
            if(timeout == 0) return 400;

            TMR1 = 0;
            T1CONbits.TMR1ON = 1;

            timeout = 10000;
            while(ECHO_LEFT == 1 && --timeout > 0);
            T1CONbits.TMR1ON = 0;
            break;

        case RIGHT_SENSOR:
            TRIG_RIGHT = 1;
            __delay_us(10);
            TRIG_RIGHT = 0;

            timeout = 10000;
            while(ECHO_RIGHT == 0 && --timeout > 0);
            if(timeout == 0) return 400;

            TMR1 = 0;
            T1CONbits.TMR1ON = 1;

            timeout = 10000;
            while(ECHO_RIGHT == 1 && --timeout > 0);
            T1CONbits.TMR1ON = 0;
            break;

        default:
            return 0;
    }

    return (TMR1 / 58);

}