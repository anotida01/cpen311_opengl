#include "gen_test_data.h"
#include <math.h>

// register your algorithms here!
int NUM_ALGORITHMS = 2;
algorithm ALGORITHMS[] = {
        {.number = 0, .function_addr = &screenfill},
        {.number = 1, .function_addr = &drawCircle}
    };

// Task 2 - Fill the screen
void screenfill(){

    char colour_byte;
    pixel p;

    for (short x = 0; x < 160; x++)
        for (short y = 0; y < 120; y++){
            
            colour_byte = x % 8;

            p.red   = (colour_byte & (1 << 2)) ? 1 : 0;
            p.green = (colour_byte & (1 << 1)) ? 1 : 0;
            p.blue  = (colour_byte & (1 << 0)) ? 1 : 0;

            // point(x, y, &c);
            // FRAMEBUFFER[y][x] = p;
            fb_write(x, y, &p);
        }

    return;
}


// Task 3 - Bresenham Circle Algorithm
void drawCircle(int centre_x, int centre_y, int radius){

    int offset_y = 0;
    int offset_x = radius;
    int crit = 1 - radius;

    int unsigned vga_x, vga_y;

    pixel c = {.red = 0, .green = 1, .blue = 0}; // green pixel

    while (offset_y <= offset_x){
        
        // point(centre_x + offset_x, centre_y + offset_y, &c); // octant 1
        // point(centre_x + offset_y, centre_y + offset_x, &c); // octant 2
        // point(centre_x - offset_x, centre_y + offset_y, &c); // octant 3
        // point(centre_x - offset_y, centre_y + offset_x, &c); // octant 3
        // point(centre_x - offset_x, centre_y - offset_y, &c); // octant 5
        // point(centre_x - offset_y, centre_y - offset_x, &c); // octant 6
        // point(centre_x + offset_x, centre_y - offset_y, &c); // octant 8
        // point(centre_x + offset_y, centre_y - offset_x, &c); // octant 7

        vga_y = centre_y + offset_y; vga_x = centre_x + offset_x; // octant 1
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

        vga_y = centre_y + offset_x; vga_x = centre_x + offset_y; // octant 2
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

        vga_y = centre_y + offset_y; vga_x = centre_x - offset_x; // octant 4
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;
   
        vga_y = centre_y + offset_x; vga_x = centre_x - offset_y; // octant 3
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

        vga_y = centre_y - offset_y; vga_x = centre_x - offset_x; // octant 5
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

        vga_y = centre_y - offset_x; vga_x = centre_x - offset_y; // octant 6
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

        vga_y = centre_y - offset_y; vga_x = centre_x + offset_x; // octant 8
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

        vga_y = centre_y - offset_x; vga_x = centre_x + offset_y; // octant 7
        if (vga_x < H_SIZE && vga_y < V_SIZE) FRAMEBUFFER[vga_y][vga_x] = c;

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
