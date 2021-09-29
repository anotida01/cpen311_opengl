/*
    File:       gen_test_data.c
    Purpose:    Create & vizualize testing data for Tasks 2, 3, 4
    Author:     anotida01@github.com
    Compilation: gcc -lGL -lglut -lGLU -lm gen_test.data.c -o gen_test_data
*/

#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "gen_test_data.h"

#define OUTFLE_NAME "task3.memh"

// FRAMEBUFFER[y][x] - y coord is row in array, x coord is column in array
// FRAMEBUFFER is organized to mirror VGA screen orientation
pixel** FRAMEBUFFER;
int FUNCTION; // used to indicate which function will be run


// write pixel to framebuffer
int fb_write (int x, int y, pixel *p){
    int status = 1;
    if ( (x < H_SIZE) && (y < V_SIZE) ){
        status = 0;
        FRAMEBUFFER[y][x] = *p;
    }
    else
        printf("[%d][%d] is out of bounds!\n", x, y);
    return status;
}

// draw a single point to openGL
inline void __gl_point(int x, int y, pixel* p){

    float diff = 1.0 / 2.0;

    glBegin(GL_QUADS);
        glColor3f(p->red, p->green, p->blue);
        glVertex2f(x - diff, y - diff);
        glVertex2f(x + diff, y - diff);
        glVertex2f(x + diff, y + diff);
        glVertex2f(x - diff, y + diff);
    glEnd();

    // flushing after every point is drawn slows down rendering
    // use to slow things down and watch
    // glFlush();

    return;

}

// paint the contents of the framebuffer with an algorithm
void __gl_display() {
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // Set OpenGL background color
    glClear(GL_COLOR_BUFFER_BIT);         // Clear OpenGL colour buffer (background)


    // populate frambuffer with an algorithm
    if (FUNCTION == 1){
        void (*fun_ptr)(void);
        fun_ptr = ALGORITHMS[0].function_addr;
        (*fun_ptr)();
    }
    else if (FUNCTION == 2){
        void (*fun_ptr)(void);
        fun_ptr = ALGORITHMS[1].function_addr;
        (*fun_ptr)();
    }


    // loop through framebuffer and draw to OpenGL
    for (size_t x = 0; x < H_SIZE; x++)
        for (size_t y = 0; y < V_SIZE; y++)
            __gl_point(x, y, &FRAMEBUFFER[y][x]);

    // Draw outline of valid framebuffer grid (V_SIZE x H_SIZE) pixels to OpenGL
    glBegin(GL_LINE_LOOP);             
        glColor3f(1.0f, 1.0f, 1.0f); // white
        glVertex2i(0, 0);    // x, y
        glVertex2i(0, V_SIZE);
        glVertex2i(H_SIZE, V_SIZE);
        glVertex2i(H_SIZE, 0);
    glEnd();
    
    // Tell OpenGL To Render
    glFlush();
}


// convert software framebuffer to VGA core testbench format
// char** vga_colour_array must be V_SIZE x H_SIZE array
void convert_fb_to_vga(char** vga_colour_array){
    char vga_colour_byte;

    for (size_t x = 0; x < H_SIZE; x++)
        for (size_t y = 0; y < V_SIZE; y++){
            vga_colour_byte = 0;

            vga_colour_byte |= (FRAMEBUFFER[y][x].red > 0.1f)   ? (1 << 2) : 0;
            vga_colour_byte |= (FRAMEBUFFER[y][x].green > 0.1f) ? (1 << 1) : 0;
            vga_colour_byte |= (FRAMEBUFFER[y][x].blue > 0.1f)  ? (1 << 0) : 0;

            vga_colour_array[y][x] = vga_colour_byte;            

        }

    write_arr_to_file(vga_colour_array);
        
    return;    

}


// write array to .memh format file
void write_arr_to_file(char** array){

    FILE* outfile = fopen(OUTFLE_NAME, "w+");
    char byte_str[3];
    int byte_int;

    // iterate over the array in row-major order
    // we do this because of the .memh format and how it gets read in ModelSim
    for (size_t y = 0; y < V_SIZE; y++)
        for (size_t x = 0; x < H_SIZE; x++){
            byte_int = array[y][x];
            sprintf(byte_str, "%x\n", byte_int);
            fprintf(outfile, byte_str);
        }

    printf("FRAMEBUFFER contents written to: %s\n", OUTFLE_NAME);
    fclose(outfile);
    return;

}


// Initialize Window & Canvas
void initGL(int argc, char** argv){
    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("OpenGL Window"); // Create a window with the given title
    glutInitWindowSize(500, 500);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutDisplayFunc(__gl_display); // Register display callback handler for window re-paint

    // setup canvas to match VGA Co-ord system
    gluOrtho2D(-10, 170.0, 130.0, -10);

    return;
}


// writes FRAMEBUFFER to OUTFILE and frees all memory allocations
void cleanup(){
    printf("OpenGL/GLUT exited. Cleaning Up Program\n");

    // setup outfile array 
    char** vga_colour_array = (char**) malloc(sizeof(char*) * V_SIZE);
    for (size_t i = 0; i < V_SIZE; i++){
        vga_colour_array[i] = (char*) malloc(sizeof(char)*H_SIZE);
    }

    // convert FRAMEBUFFER to testbench format
    convert_fb_to_vga(vga_colour_array);

    // free all allocated memory
    for (size_t i = 0; i < V_SIZE; i++){
        free(FRAMEBUFFER[i]);
        free(vga_colour_array[i]);
    }
    free(FRAMEBUFFER);
    free(vga_colour_array);
    return;
}

// Find out which algorithm should be run
int parse_args(int argc, char** argv){
    int func = 0;
    if (argc > 1){
        if (argv[1][0] == '1')
            func = 1;
        else if (argv[1][0] == '2')
            func = 2;
        else if (argv[1][0] == '3')
            func = 3;
        else
            func = 1;
    }
    else if (argc == 1){
        printf("Syntax is: %s <FUNC_NUM>\n", argv[0]);
        printf("FUNC_NUMS:\n 1 (Task 2)\n 2 (Task 3)\n 3 (Task 4)\n");
        func = 1;
    }

    printf("Running Task %d\n", func + 1);

    return func;
}


int main(int argc, char** argv){

    // find out which algorithm to run
    FUNCTION = parse_args(argc, argv);

    int frame_buff_size = 0;

    // setup the framebuffer
    FRAMEBUFFER = (pixel**) malloc(sizeof(pixel*) * V_SIZE);
    frame_buff_size += sizeof(pixel*) * V_SIZE;

    for (size_t i = 0; i < V_SIZE; i++){
        FRAMEBUFFER[i] = (pixel*) malloc(sizeof(pixel) * H_SIZE);
        frame_buff_size += sizeof(pixel) * H_SIZE;
    }

    printf("Framebuffer is %d bytes\n", frame_buff_size);

    // register cleanup() to be called when exiting
    atexit(cleanup);

    // initialize OpenGL/GLUT and enter main loop
    initGL(argc, argv);    
    glutMainLoop();

    return 0;
}
