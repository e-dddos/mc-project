/*
 * display_config.h
 *
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#ifndef DISPLAY_CONFIG_H_
#define DISPLAY_CONFIG_H_

#include <stdint.h>
#include <stdbool.h> // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>   // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>     // GPIO_PIN_X
#include <inc/hw_memmap.h>      // GPIO_PORTX_BASE

// constants for display initialization2
#define RST 0x10
#define INITIAL_STATE (0x1F)
#define SOFTWARE_RESET (0x01)
#define SET_PLL_MN (0xE2)
#define START_PLL (0xE0)
#define LOCK_PLL (0xE0)  // same as START_PLL
#define SET_LSHIFT (0xE6)
#define SET_LCD_MODE (0xB0)
#define SET_HORI_PERIOD (0xB4)
#define SET_VERT_PERIOD (0xB6)
#define SET_ADRESS_MODE (0x36)
#define SET_PIXEL_DATA_FORMAT (0xF0)
#define SET_DISPLAY_ON (0x29)
#define SET_DISPLAY_OFF (0x29) // not tested ??
uint32_t sysClock; // Variable for system clock


inline void write_command(unsigned char command);
inline void write_data(unsigned char data);
inline void window_set(unsigned int min_x, unsigned int  min_y, unsigned int  max_x, unsigned int  max_y);
void init_ports_display(void);
void configure_display_controller_small (void); // 480 x 272 pixel
void configure_display_controller_large (void); // 800 x 480 pixel ???


#endif /* DISPLAY_CONFIG_H_ */
