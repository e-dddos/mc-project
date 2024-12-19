// Testprogram two LCD Displays 480x272 and 800x480 RGB
// Display controller Type SSD 1963 => Solomon Systech
// Initialisation + Single Pixel and Rectangle Output
// ARM-Contoller Clock not changed ... to be tested
// Based partially of tests and sources from Ole Roenna 2020
// V0.1-V0.4 K.R. Riemschneider .. 1-4 Oct 2020


#include "motor.h"

/********************************************************************************/
void main(void)
{
    sysClock = SysCtlClockFreqSet(   SYSCTL_OSC_INT | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480,120000000); // Set system frequency to 120 MHz
    printf("sysClock = %d\n", sysClock);
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
    print_string("Tachometer", 50, 50, WHITE, BACKGROUND_COLOR);
    timer0A_init(200, sysClock);
    draw_tacho();
    while(1) {
        check_flag();
    }
}
/********************************************************************************/
