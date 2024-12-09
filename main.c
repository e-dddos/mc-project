// Testprogram two LCD Displays 480x272 and 800x480 RGB
// Display controller Type SSD 1963 => Solomon Systech
// Initialisation + Single Pixel and Rectangle Output
// ARM-Contoller Clock not changed ... to be tested
// Based partially of tests and sources from Ole Roenna 2020
// V0.1-V0.4 K.R. Riemschneider .. 1-4 Oct 2020


#include "draw.h"

/********************************************************************************/
void main(void)
{
    sysClock = SysCtlClockFreqSet(   SYSCTL_OSC_INT | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480,120000000); // Set system frequency to 120 MHz
    init_ports_display(); // Init Port L for Display Control and Port M for Display Data
    // Display initialization
#ifdef LARGE_DISPLAY
    configure_display_controller_large();  // initalize and  configuration
#endif
#ifdef SMALL DISPLAY
    configure_display_controller_small();  // initalize and  configuration
#endif

    set_backgound();
    // Start endless loop
    //draw_rectangles();
    
    print_string("Hello,_cruel_world!", 50, 50);
    //draw_rectangle();
}
/********************************************************************************/
