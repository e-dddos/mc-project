/*
 * draw.c
 *
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#include "draw.h"

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

void draw_rectangles(void)
{
    int i, j, x, y;
    Color color;
    color = BLUE;
    j = 0;
    while (1)
    {
        printf("Write rectangles\n"); // for debug only
        for (x = 0; x <= (MAX_X)-1 - 20; x = x + 40)
            for (y = 0; y <= (MAX_Y)-1 - 20; y = y + 40)
            {
                color = colorarray[(j) % 7];
                j++;                              // change color
                window_set(x, y, x + 20, y + 20); // set rectangle position see B.4
                write_command(0x2C);              // write pixel command
                for (i = 0; i < (40 * 40); i++)   // set pixels
                {
                    write_pixel(color);
                }
            }
    }
}

void print_char(char character, int x_pos, int y_pos, Color font_color, Color background_color)
{
    char* bitmap = char_addr[character - 32];
    char char_width = char_width_array[character - 32];
    int x, y, byte_row;
    int i, j;
    char byte = 0;
    i = 0;

    //see https://onmenwhostareongraphs.wordpress.com/2019/10/20/monochrome-bitmap-fonts-in-c-header-files/ for understanding bitmaps
    
    for (byte_row = 0; byte_row < FONT_SIZE / 8; byte_row++)
    {
        window_set(x_pos, y_pos + byte_row * 8, x_pos + char_width - 1, (y_pos + byte_row * 8) + 7); // set rectangle position see B.4
        write_command(0x2C);                                                                                 // write pixel command
        for (y = 0; y < 8; y++)
        {
            for (x = 0; x < char_width; x++)
            {
                i = x + byte_row * char_width;
                byte = bitmap[i] >> y;
                if (byte % 2 == 1)
                {
                    write_pixel(font_color);
                }
                else
                {
                    write_pixel(background_color);
                }
            }
        }
    }
}

void print_string(char* string, int x_pos, int y_pos, Color font_color, Color background_color)
{
    int i = 0;
    int x_current = x_pos;
    int y_current = y_pos;
    while (string[i] != '\0')
    {
        //printf("character = %c\n", string[i]);
        //printf("char_width_array[string[i] - 32] = %d\n", char_width_array[string[i] - 32]);
        //printf("x_current = %d\n", x_current);
        print_char(string[i], x_current, y_current, font_color, background_color);
        x_current = x_current + char_width_array[string[i] - 32] + 7;
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

//for tacho:
void draw_line_by_angle(int x1, int y1, int length, int angle_deg, Color color, int width) {
    int x0 = x1 - (int)((double)(length) * cos(((double)(angle_deg)/180) * PI));
    int y0 = y1 - (int)((double)(length) * sin(((double)(angle_deg)/180) * PI));
    //printf("x0 = %d, y0 = %d, x1 = %d, y1 = %d\n", x0, y0, x1, y1);
    draw_line(x0, y0, x1, y1, color, width);
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


void write_pixel(Color color) {
    write_data((color >> 16) & 0xff); // red
    write_data((color >> 8) & 0xff);  // green
    write_data((color) & 0xff);       // blue
}

//write single pixel at given coordinates with given color
void set_pixel(int x, int y, Color color) {
    window_set(x, y, x, y); // Set the window to the pixel position
    write_command(0x2C);    // Write pixel command
    write_pixel(color);     // Write the pixel color
}