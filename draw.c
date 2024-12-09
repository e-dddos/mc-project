/*
 * draw.c
 *
 *  Created on: 31 Oct 2024
 *      Author: edward
 */

#include "draw.h"

void set_backgound(void)
{
    printf("Start Background Pixel by Pixel set\n"); // for debug only
    // set pixel by pixel to change the background colors
    enum colors color;
    int x, y;
    color = BLUE;
    window_set(0, 0, MAX_X - 1, MAX_Y - 1); // set single position see B.4  // to do faster ?
    write_command(0x2C);                    // write pixel command
    for (x = 0; x <= (MAX_X)-1; x++)
        for (y = 0; y <= (MAX_Y)-1; y++)
        {
            write_data((color >> 16) & 0xff); // red
            write_data((color >> 8) & 0xff);  // green
            write_data((color) & 0xff);       // blue
        }
    printf("Background ready \n"); // for debug only
}

void draw_rectangles(void)
{
    int i, j, x, y;
    enum colors color;
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
                    write_data((color >> 16) & 0xff); // red
                    write_data((color >> 8) & 0xff);  // green
                    write_data((color) & 0xff);       // blue
                }
            }
    }
}

void print_char(char character, int x_pos, int y_pos)
{
    char* bitmap = char_addr[character - 33];
    char char_width = char_width_array[character - 33];
    int x, y, byte_row;
    int i, j;
    char byte = 0;
    enum colors font_color, backgroud_color;
    font_color = WHITE;
    backgroud_color = BLUE;
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
                    write_data((font_color >> 16) & 0xff); // red
                    write_data((font_color >> 8) & 0xff);  // green
                    write_data((font_color) & 0xff);       // blue
                }
                else
                {
                    write_data((backgroud_color >> 16) & 0xff); // red
                    write_data((backgroud_color >> 8) & 0xff);  // green
                    write_data((backgroud_color) & 0xff);       // blue
                }
            }
        }
    }
}

void print_string(char* string, int x_pos, int y_pos)
{
    int i = 0;
    int x_current = x_pos;
    int y_current = y_pos;
    while (string[i] != '\0')
    {
        printf("character = %c\n", string[i]);
        printf("char_width_array[string[i] - 33] = %d\n", char_width_array[string[i] - 33]);
        printf("x_current = %d\n", x_current);
        print_char(string[i], x_current, y_current);
        x_current = x_current + char_width_array[string[i] - 33] + 7;
        i++;
    }
}

void draw_rectangle(void)
{
    int x = 100;
    int y = 100;
    int i;
    enum colors color;
    color = WHITE;
    window_set(x, y, x + 50, y + 20); // set rectangle position see B.4
    write_command(0x2C);              // write pixel command
    for (i = 0; i < (51 * 21); i++)   // set pixels
    {
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff);  // green
        write_data((color) & 0xff);       // blue
    }
}
