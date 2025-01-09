/*
 * draw.c
 * 
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#include "draw.h"

//write single pixel with given color, the window must be set before using this
void write_pixel(Color color) {
    write_data((color >> 16) & 0xff); // red
    write_data((color >> 8) & 0xff);  // green
    write_data((color) & 0xff);       // blue
}

//Draw background
void set_backgound(Color color)
{
    printf("Start Background Pixel by Pixel set\n"); // for debug only
    // set pixel by pixel to change the background Color
    int x, y;
    window_set(0, 0, MAX_X - 1, MAX_Y - 1); // set single position see B.4  // to do faster ?
    write_command(0x2C);                    // write pixel command
    for (x = 0; x <= (MAX_X)-1; x++)
        for (y = 0; y <= (MAX_Y)-1; y++)
        {
            write_pixel(color);
        }
    printf("Background ready \n"); // for debug only
}

//Print character from bitmap header on a given position with given font color and background color
void print_char(char character, int x_pos, int y_pos, Color font_color, Color background_color, int font_size)
{
    int byte, y, byte_row;
    int i;
    char byte_bitmap = 0;
    i = 0;
    char* bitmap;
    char char_width;
    //We will use two font sizes, 32 and 64
    if (font_size == FONT_SIZE_SMALL) //use small font from ubuntu_bitmap_32.h
    {
        bitmap = char_addr[character - 32]; //-32 because the first available character in these bitmap headers is 32 = ' ' (space)
        char_width = char_width_array[character - 32];
    }
    else //font_size == FONT_SIZE_BIG use big font from ubuntu_bitmap_64.h
    {
        bitmap = big_char_addr[character - 32];
        char_width = big_char_width_array[character - 32];
    }
    /*see https://onmenwhostareongraphs.wordpress.com/2019/10/20/monochrome-bitmap-fonts-in-c-header-files/ for understanding bitmaps*/
    for (byte_row = 0; byte_row < font_size / 8; byte_row++)
    {
        window_set(x_pos, y_pos + byte_row * 8, x_pos + char_width - 1, (y_pos + byte_row * 8) + 7);
        write_command(0x2C);
        //For each bit in a byte
        for (y = 0; y < 8; y++)
        {
            //For each byte in a byte row
            for (byte = 0; byte < char_width; byte++)
            {
                i = byte + byte_row * char_width; //index of the byte 
                byte_bitmap = bitmap[i] >> y; //shift to the next bit
                if (byte_bitmap % 2 == 1) //check if the bit is set
                {
                    write_pixel(font_color); //set pixel for character
                }
                else
                {
                    write_pixel(background_color); //set pixel for background
                }
            }
        }
    }
}
//Print a string on given coordinates with given font color and background color
void print_string(char* string, int x_pos, int y_pos, Color font_color, Color background_color, int font_size)
{
    int i = 0;
    int x_current = x_pos;
    int y_current = y_pos;
    while (string[i] != '\0') //loop until the end of the string
    {
        print_char(string[i], x_current, y_current, font_color, background_color, font_size);
        //calculate the location of the next character of the string:
        if (font_size == FONT_SIZE_SMALL) { 
            x_current = x_current + char_width_array[string[i] - 32] + 7; //start of the next character is the end of the current character + 7
        }
        else { //font_size == FONT_SIZE_BIG
            x_current = x_current + big_char_width_array[string[i] - 32] + 10; //same as above, but +10
        }
        i++;
    }
}
//x0, y0 is the top left corner, x1, y1 is the bottom right corner
void draw_rectangle(int x0, int y0, int x1, int y1, Color color)
{
    int i;
    window_set(x0, y0, x1, y1); // set rectangle position see B.4
    write_command(0x2C);              // write pixel command
    for (i = 0; i < (((x1-x0)+1) * ((y1-y0)+1)); i++)   // set pixels
    {
        write_pixel(color);
    }
}

/*for tacho: calculates the end coordinates of a line with a given length and angle in degrees and draws it
if tacho is true, it also prints the speed numbers on the segments, because
When placing the segment lines, we have the information of where the segment line ends, it is stored in x0,y0
This is exactly the position where we want to place our speed numbers on. This way, giving the function tacho=true
as an argument, we can use this information and place the numbers there. This way we can have a variable 
number of segments and not hardcode the numbers' position*/
void draw_line_by_angle(int x1, int y1, int length, int angle_deg, Color color, int width, bool tacho) {
    int x0 = x1 - (int)((double)(length) * cos(((double)(angle_deg)/180) * PI));
    int y0 = y1 - (int)((double)(length) * sin(((double)(angle_deg)/180) * PI));
    //printf("x0 = %d, y0 = %d, x1 = %d, y1 = %d\n", x0, y0, x1, y1);
    draw_line(x0, y0, x1, y1, color, width);
    //for determining where to place the numbers on the segments and print them:
    if (tacho) {
        int speed = (angle_deg * MAX_SPEED) / 180;
        char speed_str[4];
        sprintf(speed_str, "%d", speed);
        x0 = x1 - (int)((double)(length + 30) * cos(((double)(angle_deg)/180) * PI));
        y0 = y1 - (int)((double)(length + 30) * sin(((double)(angle_deg)/180) * PI));
        print_string(speed_str, x0, y0, WHITE, BACKGROUND_COLOR, 32);
    }   
}

//draw line with given start and end coordinates of a certain color and width
void draw_line(int x0, int y0, int x1, int y1, Color color, int width) {
    //Bresenham's algorithm
    int dx = abs(x1 - x0); // Difference in x
    int dy = abs(y1 - y0); // Difference in y
    int sx = (x0 < x1) ? 1 : -1; // Step in x direction
    int sy = (y0 < y1) ? 1 : -1; // Step in y direction
    int err = dx - dy; // Error term

    while (1) {
        draw_rectangle(x0, y0, x0 + width, y0 + width, color); 
        if (x0 == x1 && y0 == y1) break; // If the end point is reached, break
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

//Draw the tachometer segments with speed numbers on them
void draw_tacho(void) {
    int i = 0;
    for (i = 0; i <= 180; i += 180/TACHO_NUM_SEGMENTS) {
        //Draw segments:      
        if (i >= 144) { //Last three segments are red:
            draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 300, i, RED, 3, true);
        } else { //Others are grey:
            draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 300, i, GREY, 3, true);
        }
        //Remove the inner parts of the segments:
        draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 250, i-1, BACKGROUND_COLOR, 3, false);
        draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 250, i, BACKGROUND_COLOR, 3, false);
        draw_line_by_angle(TACHO_CENTER_X, TACHO_CENTER_Y, 250, i+1, BACKGROUND_COLOR, 3, false);
    }
}

//Loading screen with HAW logo
void draw_haw_logo(void) {
    //Constants for HAW logo:
    int x0 = 200;
    int y0 = 170;
    int line_length = 100;
    int line_width = 6;
    int space = 30;

    int i = 0;
    int j = 0;
    set_backgound(WHITE);
    for (i = 0; i < 4; i++) {
        draw_line(x0, y0 +i*space, x0 + line_length, y0 +i*space, LIGHT_BLUE, line_width);
    }
    for (i = 0; i < 4; i++) {
        draw_line(x0 + line_length/3, y0 + line_width + 10 +i*space, x0 + line_length/3 + line_length, y0 + line_width + 10 +i*space, DARK_BLUE, line_width);
    }
    print_string("HAW", x0+150, y0, DARK_BLUE, WHITE, 64);
    print_string("HAMBURG", x0+150, y0+80, DARK_BLUE, WHITE, 64);
    print_string("Loading", x0+150, y0+200, BLACK, WHITE, 32);
    space = char_width_array['L'-32] + char_width_array['o'-32] + char_width_array['a'-32] + char_width_array['d'-32] + char_width_array['i'-32] + char_width_array['n'-32] + char_width_array['g'-32] + 7*7;
    //Fancy loading dots:
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 5000000; j++) {} //delay
        print_char('.', x0+150+space+i*(char_width_array['.'-32]+7), y0+200, BLACK, WHITE, 32);
    }
}

void draw_button(void) {
    draw_rectangle(50, 170, 180, 230, RED); //Draw outline
    draw_rectangle(52, 172, 178, 228, WHITE); //Draw inner part
    print_string("RESET", 70, 185, RED, WHITE, 32);
}