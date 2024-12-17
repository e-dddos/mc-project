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
    int i,j;
    sysClock = SysCtlClockFreqSet(   SYSCTL_OSC_INT | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480,120000000); // Set system frequency to 120 MHz
    init_ports_display(); // Init Port L for Display Control and Port M for Display Data
    // Display initialization
#ifdef LARGE_DISPLAY
    configure_display_controller_large();  // initalize and  configuration
#endif
#ifdef SMALL DISPLAY
    configure_display_controller_small();  // initalize and  configuration
#endif

    set_backgound(BLUE); // set background color
    // Start endless loop
    //draw_rectangles();
    
    print_string("Blue screen of death", 50, 50, WHITE, BLUE);
    print_string(":(", 50, 100, WHITE, BLUE);
    //draw_line(50, 150, 200, 250, WHITE, 1);
    //draw_line_by_angle(200, 250, 100, 200, WHITE, 1);
    i = 0;
    while(1)
    {
        draw_line_by_angle(200, 250, 100, i % 360, WHITE, 1);
        //need timer here:
        for (j=0;j<100000;j++);
        draw_line_by_angle(200, 250, 100, i % 360, BLUE, 1);
        i++;
    }
    //draw_rectangle();
}
/********************************************************************************/
