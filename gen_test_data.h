/*
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

// Vertical & Horizontal sizes in pixels
#define V_SIZE  120
#define H_SIZE  160

// FRAMEBUFFER[y][x] - y coord is row in array, x coord is column in array
// FRAMEBUFFER is organized to mirror VGA screen orientation
extern pixel** FRAMEBUFFER;
extern int FUNCTION; // used to indicate which function will be run
extern algorithm ALGORITHMS[];

// EDIT HERE - ALGORITHMS
void screenfill();
void dog();
void drawCircle();

int fb_write (int x, int y, pixel *p);

// Internal functions
int __parse_args(int argc, char** argv);
void __gl_display();
void __initGL(int argc, char** argv);
void __gl_point(int x, int y, pixel* p);
void __write_arr_to_file(char** array);
void __cleanup();

#endif
