/*
    File:       gen_test_data.h
    Purpose:    Header file for gen_test_data.c
    Author:     anotida01@github.com
*/

#ifndef __GEN_TEST_DATA_H__
#define __GEN_TEST_DATA_H__

typedef struct pixel{
    int red;
    int green;
    int blue;
} pixel;

typedef struct algorithm{
    int number;
    void* function_addr;
}algorithm;

extern algorithm ALGORITHMS[];

#define V_SIZE  120
#define H_SIZE  160

// FRAMEBUFFER[y][x] - y coord is row in array, x coord is column in array
// FRAMEBUFFER is organized to mirror VGA screen orientation
extern pixel** FRAMEBUFFER;
extern int FUNCTION; // used to indicate which function will be run

void __gl_point(int x, int y, pixel* p);
int fb_write (int x, int y, pixel *p);
void screenfill();
void display();
void initGL(int argc, char** argv);
void write_arr_to_file(char** array);
void cleanup();
int parse_args(int argc, char** argv);
void drawCircle(int centre_x, int centre_y, int radius);
void reuleaux_triangle(int centre_x, int centre_y, int diameter);
void drawCircle_t4(int centre_x, int centre_y, int radius, int* c_x, int* c_y, int mode);
char should_plot(int mode, int* c_x, int* c_y, int unsigned vga_x, int unsigned vga_y);

#endif
