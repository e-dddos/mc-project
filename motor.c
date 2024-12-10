/*
 * motor.c
 *
 *  Created on: 28 Nov 2024
 *      Author: edward
 */

#include "motor.h"

#include "calc.h" //local calc of speed

bool flag = false;


void configure_gpios(void){
        // Set Port P Pins 0,1: 0 - S1, 1 - S2
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);            // enable clock-gate Port P
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOP));     // wait until clock ready
        GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1);            // set Pin 0 and 1 as Input

        GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_PIN_0, GPIO_RISING_EDGE); // set Pin 0 of Port P (S1) to trigger an interrupt on a rising edge

        // "register" entry in  a copied IVT
        GPIOIntRegister(GPIO_PORTP_BASE, isr_s1);
        GPIOIntClear(GPIO_PORTP_BASE,GPIO_PIN_0); // optional ...
        IntPrioritySet(INT_GPIOP0,0x20); //high prio
        GPIOIntEnable(GPIO_PORTP_BASE, GPIO_PIN_0); // Allow request output from Port unit
        IntEnable(INT_GPIOP0);  // Allow request input to NVIC
}


void isr_s1(void) {
    flag = true;
    GPIOIntClear(GPIO_PORTP_BASE,GPIO_PIN_0);
}


void check_flag(void) {
    if (flag) {
                printf("Interrupt!\n");
                flag = false;
            }
}


//aux for interrupt related reading functions

void readFrequency()
{
        //init timer and run for approx 1 sec, after next interrupt stop and return to speed func 
}

int readDirection();
{
        //activate after 00 pulse in interrupt and return pulse from s1 (can be changed to s2, doesn't really matter)
         return GPIO_PORTP_BASE;   //1 means forward, 0 means backwards
}

void dailyDistance()
{
        //every time speed() is called, add time * speed
        x = 1; //x is the approx. time needed for an interrupt
        //timer init

        dailyDistanceCount += x * speed(readFrequency());
}
