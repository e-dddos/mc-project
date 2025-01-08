/*
 * motor.c
 *
 *  Created on: 28 Nov 2024
 *      Author: edward
 */

#include "motor.h"

#include "calc.h" //local calc of speed

bool flag = false;
uint16_t curr_spin_count, temp_spin_count = 0;
double dailyDistance = 0;

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
    curr_spin_count++;
    GPIOIntClear(GPIO_PORTP_BASE,GPIO_PIN_0);
}


void check_flag(void) {
    uint16_t temp_spin_count = 0;

    if (flag) {
                temp_spin_count = curr_spin_count;
                printf("Interrupt from timer! spin_count=%d\n", temp_spin_count);

                dailyDistance(temp_spin_count);
            
                curr_spin_count = 0;
                flag = false;
            }
}

void timer0A_init(uint32_t milliseconds, uint32_t sysClock) {
    // Enable Timer0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Wait for the Timer0 module to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) {}
    // Configure Timer0 as a 32-bit periodic timer
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Calculate load value for the specified milliseconds
    uint32_t loadValue = (sysClock / 1000) * milliseconds - 1;
    // Set the timer load value
    TimerLoadSet(TIMER0_BASE, TIMER_A, loadValue);
    // Register the interrupt handler
    TimerIntRegister(TIMER0_BASE, TIMER_A, timer0A_isr);
    // Enable Timer0A timeout interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Enable Timer0A interrupt in the NVIC
    IntEnable(INT_TIMER0A);
    // Enable global interrupts
    //IntMasterEnable();
    // Enable Timer0A
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// Interrupt Service Routine for Timer0A
void timer0A_isr(void) {
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    flag = true;
    // Your logic here: what to do when the timer expires
    // For example, toggle an LED or set a flag
}

//aux for interrupt related reading functions

double dailyDistance(int spin)
{
        uint8_t wheelDiameter = 1; //wie gross man das Rad haben moechte
        //every time speed() is called, add time * speed
        //x = 1; //x is the approx. time needed for an interrupt
        //timer init
        if(1 == 0)
        {
                //falls reset gedrueckt wird (muss noch implementiert werden)
                distanceToday = 0;
        }
        else
        {
                distanceToday = (distanceToday + (wheelDiameter * spin)) / 3600; //Zaehlt die daily distance hoch
        }
        //dailyDistanceCount += x * speed(readFrequency());
        return distanceToday;
}
