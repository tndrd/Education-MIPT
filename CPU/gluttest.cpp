#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

int window_size = 200;

void DrawPoint(int x, int y, double color){
       glPointSize(10);
       glColor3d(1.0,1.0,0);
       glBegin(GL_POINTS);
       glVertex2f(x, y);
       glEnd();
}



void display(double* RAM)
     {
      glClear(GL_COLOR_BUFFER_BIT);
      for (int npixel = 0; npixel < window_size*window_size; npixel++){
        DrawPoint(npixel / window_size, npixel % window_size, RAM[npixel]);
        printf("Drawn on %d %d: %lf\n", 1, 160, RAM[360]);
      }
      glutSwapBuffers();
     }           

void 

int main(int argc, char **argv)
{
    double* RAM = (double*)calloc(window_size*window_size,sizeof(double));
    RAM[360] = 1.0;
    RAM[570] = 1.0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(window_size,window_size);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Window");
    glClearColor(1, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -100, 100);
    display(RAM);

    return 0;
}