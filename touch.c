
// Based on
// Example and Testprogram  Touch Function
// See Installation Manual
// K.R. Riemschneider v.04  2023

#include "touch.h"

void touch_write(unsigned char value)
{
	unsigned char i = 0x08; // 8 bit command
	unsigned char x, DI;
	GPIO_PORTD_AHB_DATA_R &= 0xFB; //CS=0
	while (i > 0) {
		DI = (value >> 7);
		if (DI == 0) {GPIO_PORTD_AHB_DATA_R &= 0xfe;} //out bit=0
		else {GPIO_PORTD_AHB_DATA_R |= 0x01;} //out bit=1
		value <<= 1; //next value
		GPIO_PORTD_AHB_DATA_R |= 0x08; //Clk=1
		for (x = 0; x < 10; x++);
		GPIO_PORTD_AHB_DATA_R &= 0xf7; //Clk=0
		for (x = 0; x < 10; x++);
		i--;
	}
}

unsigned int touch_read()
{
	unsigned char i = 12; // 12 Bit ADC
	unsigned int x, value = 0x00;
	while (i > 0)
	{
		value <<= 1;
		GPIO_PORTD_AHB_DATA_R |= 0x08; //Clk=1
		for (x = 0; x < 10; x++);
		GPIO_PORTD_AHB_DATA_R &= 0xf7; //Clk=0
		for (x = 0; x < 10; x++);
		value |= ((GPIO_PORTD_AHB_DATA_R >> 1) & 0x01); // read value
		i--;
	}
	GPIO_PORTD_AHB_DATA_R |= 0x04; //CS=1
	return value;
}

void check_button(void)
{
    int x, xpos, ypos;
        touch_write(0xD0);         			//Touch Command XPos read
        for (x = 0; x < 10; x++);  			//Busy wait
        xpos = touch_read();       			//xpos value read ( 0......4095 )
        touch_write(0x90);         			//Touch Command YPos read
        for (x = 0; x < 10; x++);  			//Busy wait
        ypos = touch_read();       			//ypos value read ( 0.....4095 )
        //If RESET button is pressed
        if (xpos > 3100 && xpos < 3700 && ypos > 1500 && ypos < 1800) {
            reset_distance();
        }
}