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
uint16_t curr_spin_count = 0, temp_spin_count = 0, total_spin_count = 0;
uint8_t dir = 0; //0 - vorw, 1 - rueckw
uint8_t dir_save = 1;

//Configure Display, GPIOs, draw loading screen and draw tacho
void system_init(void) {
    int i = 0;
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
    timer0A_init(200, sysClock); //set timer to send interrupt every 200ms
    draw_haw_logo(); //Draw loading screen
    while (i < 10) { //wait for 2 seconds
        if (timer_flag) {
            i++;
            timer_flag = false;
        }
    }
    set_backgound(BACKGROUND_COLOR); // set background color
    print_string("Tachometer", 50, 50, WHITE, BACKGROUND_COLOR, FONT_SIZE_SMALL);
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
                //500 is about the highest rotation count per second, the *5 comes because we get the number of rotations every 1/5 s = 200ms
                //so for example if temp_spin_count=50, the angle of the tacho pointer will be be in the middle (90°)
                total_spin_count += curr_spin_count;
                draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 220, temp_spin_count*5*180/500, BACKGROUND_COLOR, 1, false); //remove the previous tacho pointer
                temp_spin_count = curr_spin_count;
                curr_spin_count = 0;
                draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 220, temp_spin_count*5*180/500, RED, 1, false); //draw the new tacho pointer
                timer_flag = false;
                //Only update direction if it has changed
                if (dir != dir_save) {
                    dir_save = dir;
                    draw_rectangle(700, 100, 750, 130, BACKGROUND_COLOR); //to remove the rest of 'V' after changing to 'R'
                    print_char('V'-4*dir,700, 100, GREEN, BACKGROUND_COLOR, FONT_SIZE_BIG); //'V' - 4*1 = 'R'. if dir = 1, 'R' will be printed
                }
                print_total_distance();
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
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Clear the timer interrupt
    timer_flag = true; //set timer flag
}

void print_total_distance(void) {
    char distance_str[10];
    sprintf(distance_str, "%.2f km", total_spin_count * 0.25 * 0.001); //0.25m per rotation. 0.001 to convert to km
    print_string(distance_str, 50, 100, WHITE, BACKGROUND_COLOR, FONT_SIZE_SMALL);
}