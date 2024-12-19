/*
 * motor.h
 *
 *  Created on: 28 Nov 2024
 *      Author: edward
 */

#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_

#include "inc/tm4c1294ncpdt.h" // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h" //  Tivaware Macros ...BASE as base address for peripheral modules

// requirements for Tiva gpio.h and sysctl.h
#include <stdint.h>              // Define varios  integer types
#include <stdio.h>
#include <stdbool.h>             // handling logical boolean strict False=0 and True=1 (C++)
                                   // sloppy in old C style False==0 and True== anything except O
#include "driverlib/systick.h"   //  Tivaware functions: SysTick...
#include "driverlib/interrupt.h" //  Tivaware functions: Int...
#include "driverlib/sysctl.h"    //  Tivaware functions: SysCtl...  + Macros  SYSCTL_...
#include "driverlib/gpio.h"      //  Tivaware functions: GPIO... + Macros  GPIO_...
//For timer:
#include "driverlib/timer.h"

#include "draw.h"


void configure_gpios(void);
void timer0A_init(uint32_t milliseconds, uint32_t sysClock);
void timer0A_isr(void);

//void configure_timer(void);
void check_flag(void);

void isr_s1(void);

#endif /* SRC_MOTOR_H_ */
