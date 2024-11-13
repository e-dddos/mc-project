/*
 * draw.h
 *
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#ifndef DRAW_H_
#define DRAW_H_

#include "display_config.h"


// Size of Display define
#define LARGE_DISPLAY
// #define SMALL_DISPLAY


#ifdef LARGE_DISPLAY
    #define MAX_X 800
    #define MAX_Y 480
    #endif
    #ifdef SMALL DISPLAY
    #define MAX_X 480
    #define MAX_Y 272
    #endif


/* some predefined basic colors to use with names */
enum colors{BLACK=0x00000000,WHITE=0x00FFFFFF,GREY=0x00AAAAAA,RED=0x00FF0000,GREEN=0x0000FF00,BLUE=0x000000FF,YELLOW=0x00FFFF00, LOL=0x00AAFFAA};
/* same values as array for indexed colors */
static int colorarray[]={0x00000000,0x00FFFFFF,0x00AAAAAA,0x00FF0000,0x0000FF00,0x000000FF,0x00FFFF00};


void set_backgound(void);
void draw_rectangles(void);


#endif /* DRAW_H_ */
