/*
 * motor.c
 *
 *  Created on: 28 Nov 2024
 *      Author: edward
 */

#include "tacho.h"
#include "calc.h" //local calc of speed

//Global variables
bool timer_flag = false;
uint16_t curr_spin_count, temp_spin_count  = 0;
uint8_t dir = 0; //0 - vorw, 1 - rueckw
uint8_t dir_save = 1;


void system_init(void) {
    sysClock = SysCtlClockFreqSet(   SYSCTL_OSC_INT | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480,120000000); // Set system frequency to 120 MHz
    init_ports_display(); // Init Port L for Display Control and Port M for Display Data
    // Display initialization
    #ifdef LARGE_DISPLAY
        configure_display_controller_large();  // initalize and  configuration
    #endif
    #ifdef SMALL DISPLAY
        configure_display_controller_small();  // initalize and  configuration
    #endif
    configure_gpios();
    set_backgound(BACKGROUND_COLOR); // set background color
    print_string("Tachometer", 50, 50, WHITE, BACKGROUND_COLOR, FONT_SIZE_SMALL);
    timer0A_init(200, sysClock); //set timer to send interrupt every 200ms
    draw_tacho();
}
//configure GPIO as Inputs and define ISR for S1
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
//ISR for S1: increment spin count and read S2 for direction: if S2 is high, direction is forward (R), else backwards (V)
void isr_s1(void) {
    GPIOIntClear(GPIO_PORTP_BASE,GPIO_PIN_0);
    curr_spin_count++;
    dir = (GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_1) & GPIO_PIN_1) >> 1;
}

//Redraw tacho pointer every time timer flag is set, update direction if it has changed
void run_tacho(void) {

    if (timer_flag) {
                draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 200, temp_spin_count*5*180/500, BACKGROUND_COLOR, 3, false);
                temp_spin_count = curr_spin_count;
                curr_spin_count = 0;
                draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 200, temp_spin_count*5*180/500, WHITE, 3, false);
                timer_flag = false;
                //Only update direction if it has changed
                if (dir != dir_save) {
                    dir_save = dir;             
                    if (dir) {
                        draw_rectangle(500, 50, 550, 80, BACKGROUND_COLOR);
                        print_char('R',500, 50, WHITE, BACKGROUND_COLOR, FONT_SIZE_BIG);
                    } else {
                        print_char('V',500, 50, WHITE, BACKGROUND_COLOR, FONT_SIZE_BIG);
                    }
                }
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
    timer_flag = true;
}

//aux for interrupt related reading functions

void readFrequency()
{
        //init timer and run for approx 1 sec, after next interrupt stop and return to speed func 
}

int readDirection()
{
        //activate after 00 pulse in interrupt and return pulse from s1 (can be changed to s2, doesn't really matter)
         //return GPIO_PORTP_BASE;   //1 means forward, 0 means backwards
}

void dailyDistance()
{
        //every time speed() is called, add time * speed
        //x = 1; //x is the approx. time needed for an interrupt
        //timer init

        //dailyDistanceCount += x * speed(readFrequency());
}
