#include "main.h"
#include "dog.h"
#include <math.h>

// register your algorithms here!
int NUM_ALGORITHMS = 3;
algorithm ALGORITHMS[] = {
        {.number = 0, .function_addr = &screenfill},
        {.number = 1, .function_addr = &drawCircle},
        {.number = 2, .function_addr = &dog}
        // {.number = , .function_addr = &}
    };

// Fill the screen with colours
void screenfill(){

    char colour_byte;
    pixel p;

    for (short x = 0; x < 160; x++)
        for (short y = 0; y < 120; y++){
            
            colour_byte = x % 8;

            p.red   = (colour_byte & (1 << 2)) ? 1 : 0;
            p.green = (colour_byte & (1 << 1)) ? 1 : 0;
            p.blue  = (colour_byte & (1 << 0)) ? 1 : 0;

            fb_write(x, y, &p);
        }

    return;
}

// Draw a dog!
void dog(){

    pixel p = {.red = 1, .green = 1, .blue = 0};

    for (int i = 0; i < DOG_NUM; i++){
        unsigned char x = DOG[i][0];
        unsigned char y = DOG[i][1];
        fb_write(x, y, &p);
    }

    return;

}

// parameters to drawCircle
int CENTRE_X = 80;
int CENTRE_Y = 60;
int RADIUS = 40;

// Bresenham Circle Algorithm
void drawCircle(){

    int centre_x = CENTRE_X; 
    int centre_y = CENTRE_Y;
    int radius = RADIUS;

    int offset_y = 0;
    int offset_x = radius;
    int crit = 1 - radius;

    int unsigned vga_x, vga_y;

    pixel p = {.red = 0, .green = 1, .blue = 0}; // green pixel

    while (offset_y <= offset_x){

        vga_y = centre_y + offset_y; vga_x = centre_x + offset_x; // octant 1
        fb_write(vga_x, vga_y, &p);

        vga_y = centre_y + offset_x; vga_x = centre_x + offset_y; // octant 2
        fb_write(vga_x, vga_y, &p);

        vga_y = centre_y + offset_y; vga_x = centre_x - offset_x; // octant 4
        fb_write(vga_x, vga_y, &p);
   
        vga_y = centre_y + offset_x; vga_x = centre_x - offset_y; // octant 3
        fb_write(vga_x, vga_y, &p);

        vga_y = centre_y - offset_y; vga_x = centre_x - offset_x; // octant 5
        fb_write(vga_x, vga_y, &p);

        vga_y = centre_y - offset_x; vga_x = centre_x - offset_y; // octant 6
        fb_write(vga_x, vga_y, &p);

        vga_y = centre_y - offset_y; vga_x = centre_x + offset_x; // octant 8
        fb_write(vga_x, vga_y, &p);

        vga_y = centre_y - offset_x; vga_x = centre_x + offset_y; // octant 7
        fb_write(vga_x, vga_y, &p);

        offset_y = offset_y + 1;
        if (crit <= 0)
            crit = crit + (2 * offset_y) + 1;
        else{
            offset_x = offset_x - 1;
            crit = crit + 2 * (offset_y - offset_x) + 1;
        }

    }

    return;

}
