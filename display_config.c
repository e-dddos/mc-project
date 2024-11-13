/*
 * display_config.c
 *
 *  Created on: 31 Oct 2024
 *      Author:
 */

#include "display_config.h"

/********************************************************************************
     Elementary output functions  => speed optimized as inline
*********************************************************************************/
inline void write_command(unsigned char command)
{   GPIO_PORTM_DATA_R = command;        // Write command byte
    GPIO_PORTL_DATA_R = 0x11;           // Chip select = 0, Command mode select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
}
/********************************************************************************/
inline void write_data(unsigned char data)
{   GPIO_PORTM_DATA_R = data;           // Write data byte
    GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
}
/********************************************************************************/
inline void window_set(unsigned int min_x, unsigned int min_y, unsigned int max_x, unsigned int max_y)
{
    write_command(0x2A);           // Set row address x-axis
    write_data(min_x >> 8);        // Set start  address           (high byte)
    write_data(min_x);             // as above                     (low byte)
    write_data(max_x >> 8);        // Set stop address             (high byte)
    write_data(max_x);             // as above                     (low byte)
    write_command(0x2B);           // Set column address (y-axis)
    write_data(min_y >> 8);        // Set start column address     (high byte)
    write_data(min_y);             // as above                     (low byte)
    write_data(max_y >> 8);        // Set stop column address      (high byte)
    write_data(max_y);             // as above                     (low byte)
}
/********************************************************************************/
void init_ports_display(void)
{
    // Set Port M Pins 0-7: used as Output of LCD Data
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);            // enable clock-gate Port M
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));     // wait until clock ready
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, 0xFF);
    // Set Port L Pins 0-4: used as Output of LCD Control signals:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);  // Clock Port Q
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4);
}
/********************************************************************************/
void configure_display_controller_small (void) // 480 x 272 pixel
{
    GPIO_PORTL_DATA_R = INITIAL_STATE;      // Initial state of display control signals
    GPIO_PORTL_DATA_R &= ~RST;              // Display hardware reset prepare, manual p. 79
    SysCtlDelay(10000);                     // wait >1 ms
    GPIO_PORTL_DATA_R |= RST;               // Display hardware reset trigger
    SysCtlDelay(10000);                     // wait >1 ms

    write_command(SOFTWARE_RESET);          // Display Software reset, manual p. 24
    SysCtlDelay(100000);                    // wait >5 ms

    write_command(SET_PLL_MN);              // Set PLL Freq of display controller to 100MHz
    write_data(0x1D);                       // see manual p. 74
    write_data(0x02);                       //
    write_data(0x04);                       //

    write_command(START_PLL);               // Start PLL, see manual p. 73
    write_data(0x01);                       //
    SysCtlDelay(10000);                     // wait >1 ms

    write_command(LOCK_PLL);               // Lock PLL, see manual p. 73/74
    write_data(0x03);                       //
    SysCtlDelay(10000);                     // wait >1 ms

    write_command(SOFTWARE_RESET);          // Software reset, manual p. 24
    SysCtlDelay(100000);                    // wait > 10 ms

    write_command(SET_LSHIFT);              // Pixel clock frequency
    write_data(0x01);                       // set pixel Clock 9MHz, see manual p. 76
    write_data(0x70);                       //
    write_data(0xA3);                       //

    write_command(SET_LCD_MODE);            // Set LCD Panel mode, see manual p. 76
    write_data(0x20);                       // TFT panel 24bit
    write_data(0x00);                       // TFT mode
    write_data(0x01);                       // Set horizontal size = 480-1 (high byte)
    write_data(0xDF);                       // Set horizontal size = 480-1 (low byte)
    write_data(0x01);                       // Set vertical size = 272-1  (high byte)
    write_data(0x0F);                       // Set vertical size = 272-1 (low byte)
    write_data(0x00);                       // Even line RGB sequence / Odd line RGB sequence RGB

    write_command(SET_HORI_PERIOD);         // Set horizontal periods, manual  p. 47
    write_data(0x02);                       // Horizontal total period (display + non-displayed) in pixels (high byte)
    write_data(0x13);                       // as above (low byte) = total 531 pixels
    write_data(0x00);                       // Non-displayed period between the start of the horizontal sync (LLINE) signal
                                            // and the first display data  (high byte)
    write_data(0x2B);                       // as above (low byte) = total sync pulse start position  is 43 pixels
    write_data(0x0A);                       // Set the horizontal sync width = 10 pixels
    write_data(0x00);                       // Set horizontal sync pulse start (high byte)
    write_data(0x08);                       // as above (low byte) = total sync pulse start position is 8 pixels
    write_data(0x00);                       //

    write_command(SET_VERT_PERIOD);         // Set vertical periods, manual  p. 49
    write_data(0x01);                       // Vertical total period (display + non-displayed) in lines (high byte)
    write_data(0x20);                       // as above (low byte) = total 288 lines
    write_data(0x00);                       //
    write_data(0x0C);                       // The non-displayed period in lines between the start of the frame and the first
                                            // display data in line = 12 pixels.
    write_data(0x0A);                       // Set the horizontal sync width = 10 pixels
    write_data(0x00);                       // Set vertical sync pulse start position (high byte)
    write_data(0x04);                       // as above (low byte) = total sync pulse start position is 4 lines

    write_command(SET_ADRESS_MODE);         // Pixel address counting = flip display , manual p. 36
    write_data(0x03);                       // necessary to match with touch screen addressing

//  write_command(0x0A);                    // Power control mode not tested in detail
//  write_data(0x1C);

    write_command(SET_PIXEL_DATA_FORMAT);    // set pixel data format 8bit manual p. 78
    write_data(0x00);

    write_command(SET_DISPLAY_ON);           // Set display on  manual p. 78
}
/********************************************************************************/
void configure_display_controller_large (void) // 800 x 480 pixel ???
{
//////////////////////////////////////////////////////////////////////////////////
    GPIO_PORTL_DATA_R = INITIAL_STATE;      // Initial state
    GPIO_PORTL_DATA_R &= ~RST;              // Hardware reset
    SysCtlDelay(10000);                     // wait >1 ms
    GPIO_PORTL_DATA_R |= RST;               //
    SysCtlDelay(12000);                     // wait >1 ms

    write_command(SOFTWARE_RESET);          // Software reset
    SysCtlDelay(120000);                    // wait >10 ms

    write_command(SET_PLL_MN);               // Set PLL Freq to 120 MHz
    write_data(0x24);                        //
    write_data(0x02);                        //
    write_data(0x04);                        //

    write_command(START_PLL);                // Start PLL
    write_data(0x01);                        //
    SysCtlDelay(10000);                      // wait 1 ms

    write_command(START_PLL);                // Lock PLL
    write_data(0x03);                        //
    SysCtlDelay(10000);                      // wait 1 ms

    write_command(SOFTWARE_RESET);           // Software reset
    SysCtlDelay(100000);                     // wait 10 ms

    write_command(0xe6);                    // Set pixel clock frequency
    write_data(0x01);                       // KRR Set LCD Pixel Clock 9MHz
    write_data(0x70);                       // KRR
    write_data(0xA3);                       // KRR

    write_command(SET_LCD_MODE);          // SET LCD MODE SIZE, manual p. 44
    write_data(0x20);                     // ..TFT panel 24bit
    write_data(0x00);                     // ..TFT mode
    write_data(0x03);                     // SET horizontal size = 800-1 (high byte)
    write_data(0x1F);                     // SET horizontal size = 800-1 (low byte)
    write_data(0x01);                     // Set vertical size = 480-1 (high byte)
    write_data(0xDF);                     // Set vertical size = 480-1 (low byte)
    write_data(0x00);                     // Even line RGB sequence / Odd line RGB sequence RGB

    write_command(SET_HORI_PERIOD);       // Set Horizontal Period
    write_data(0x03);                     // Horizontal total period (display + non-displayed)  (high byte)
    write_data(0x5E);                     // Horizontal total period (display + non-display) (low byte)
    write_data(0x00);                     // Non-displayed period between the start of the horizontal sync (LLINE) signal and the first displayed data.
    write_data(0x46);                     // Low byte of the non-display period between the start of the horizontal sync (LLINE) signal and the first display data
    write_data(0x09);                     // Set the vertical sync width
    write_data(0x00);                     // Set horiz.Sync pulse start    (high byte)
    write_data(0x08);                     // Set horiz.Sync pulse start    (low byte)
    write_data(0x00);                     //

    write_command(SET_VERT_PERIOD);         // Set vertical periods, manual  p. 49
    write_data(0x01);                       // Vertical total period (display + non-displayed) in lines (high byte)
    write_data(0xFE);                       // as above (low byte) = total 510  lines
    write_data(0x00);                       //
    write_data(0x0C);                       // The non-displayed period in lines between the start of the frame and the first
                                            // display data = 12 line.s
    write_data(0x0A);                       // Set the vertiacla sync width = 10 pixels
    write_data(0x00);                       // Set vertical sync pulse start position (high byte)
    write_data(0x04);                       // as above (low byte) = total sync pulse start position is 4 lines

    write_command(SET_ADRESS_MODE);         // Pixel address counting = flip display , manual p. 36
    write_data(0x03);                       // necessary to match with touch screen addressing

//  write_command(0x0A);                    // Power control mode not tested in detail
//  write_data(0x1C);

    write_command(SET_PIXEL_DATA_FORMAT);    // set pixel data format 8bit manual p. 78
    write_data(0x00);

    write_command(SET_DISPLAY_ON);           // Set display on  manual p. 78
}


