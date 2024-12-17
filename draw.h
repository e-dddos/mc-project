/*
 * draw.h
 *
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#ifndef DRAW_H_
#define DRAW_H_

#include <math.h>
#include "display_config.h"
#include "ubuntu_bitmap_32.h"

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

#define FONT_SIZE 32
#define PI 3.14159265
/* some predefined basic colors to use with names */
typedef enum {BLACK=0x00000000,WHITE=0x00FFFFFF,
GREY=0x00AAAAAA,RED=0x00FF0000,GREEN=0x0000FF00,
BLUE=0x000000FF,YELLOW=0x00FFFF00, LOL=0x00AAFFAA} Color;
/* same values as array for indexed colors */
static int colorarray[]={BLACK,WHITE,GREY,RED,GREEN,BLUE,YELLOW};

//static const char bitmap_72[] = {0,0,0,248,248,248,0,0,0,0,0,0,0,0,0,0,0,0,0,248,248,248,0,0,0,0,0,255,255,255,96,96,96,96,96,96,96,96,96,96,96,96,96,255,255,255,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,0,0};
//static const char char_width = 24;

void set_backgound(Color color);
void draw_rectangles(void);
void print_char(char character, int x_pos, int y_pos, Color font_color, Color background_color);
void print_string(char* string, int x_pos, int y_pos, Color font_color, Color background_color);
void draw_rectangle(int x0, int y0, int x1, int y1, Color color);
void draw_line(int x0, int y0, int x1, int y1, Color color, int width);
void draw_line_by_angle(int x1, int y1, int length, int angle_deg, Color color, int width);
void set_pixel(int x, int y, Color color);
void write_pixel(Color color);

#endif /* DRAW_H_ */
