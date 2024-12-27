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
#include "ubuntu_bitmap_64.h"

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

#define FONT_SIZE_SMALL 32
#define FONT_SIZE_BIG 64
#define PI 3.14159265
#define TACHO_CENTER_X MAX_X/2
#define TACHO_CENTER_Y 450
#define TACHO_NUM_SEGMENTS 10
#define BACKGROUND_COLOR DARK_BLUE
#define MAX_SPEED 400

/* some predefined basic colors to use with names */
typedef enum {BLACK=0x00000000,WHITE=0x00FFFFFF,
GREY=0x00AAAAAA,RED=0x00FF0000,GREEN=0x0000FF00,
BLUE=0x000000FF,YELLOW=0x00FFFF00, DARK_BLUE = 0x00000044} Color;

void set_pixel(int x, int y, Color color);
void write_pixel(Color color);
void set_backgound(Color color);
void print_char(char character, int x_pos, int y_pos, Color font_color, Color background_color, int font_size);
void print_string(char* string, int x_pos, int y_pos, Color font_color, Color background_color, int font_size);
void draw_rectangle(int x0, int y0, int x1, int y1, Color color);
void draw_line(int x0, int y0, int x1, int y1, Color color, int width);
void draw_line_by_angle(int x1, int y1, int length, int angle_deg, Color color, int width, bool tacho);
void draw_tacho(void);
#endif /* DRAW_H_ */
