/*
 * draw.c
 *
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#include "draw.h"


void set_backgound(void) {
    printf("Start Background Pixel by Pixel set\n"); // for debug only
    // set pixel by pixel to change the background colors
    enum colors color;
    int x,y;
    color=RED;
    window_set(0,0,MAX_X-1,MAX_Y-1); // set single position see B.4  // to do faster ?
    write_command(0x2C); //write pixel command
    for (x=0;x<=(MAX_X)-1;x++)
        for (y=0;y<=(MAX_Y)-1;y++)
        {
            write_data((color>>16)&0xff); // red
            write_data((color>>8)&0xff); // green
            write_data((color)&0xff); // blue
        }
    printf("Background ready \n"); // for debug only
}

void draw_rectangles(void) {
    int i,j,x,y;
    enum colors color;
    color = RED;
    j=0;
    // Start endless loop
    while (1) {
        //printf("Write rectangles\n"); // for debug only
        for (x=0;x<=(MAX_X)-1-20;x=x+40)
            for (y=0;y<=(MAX_Y)-1-20;y=y+40)
            {
                color=colorarray[(j)%7]; j++; // change color
                window_set(x,y,x+20,y+20); // set rectangle position see B.4
                write_command(0x2C); //write pixel command
                for(i=0;i<(40*40);i++) // set pixels
                {
                    write_data((color>>16)&0xff); // red
                    write_data((color>>8)&0xff); // green
                    write_data((color)&0xff); // blue
                }
            }
    }
}


