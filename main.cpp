#include <GL/glut.h>
#include <cmath>
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;
//class StepanPlot {
//public:
//};

static auto func_cos(double x) {
    return  cos(x/20);
}

static auto func_sin(double x) {
    return  sin(x/20);
}

static double (*func)(double);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    {
        glColor3f(1,0,0);
        glVertex2f(-20, 0);
        glVertex2f(500, 0);
        glColor3f(0,1, 0);
        glVertex2f(0, -100);
        glVertex2f(0, 100);
    }
    glEnd();
    glBegin(GL_POINTS);
    glColor3f(1, .2, 0);
    for (double x = 0; x < 450; x+=0.1) {
        glVertex2d(x - 0.1, func_cos(x - 0.1));
        glVertex2d(x, func(x));
    }
    glEnd();
    glFlush();
    sleep(1);
    glutDestroyWindow(0);
}

void init() {
    glClearColor(0.25, 0.0, 0.2, 1.0);
    glColor3f(0.6, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 500.0, -1.2, 1.2, 0.0, 100.0);
}

void test(int argc, char** argv, double (*function)(double)) {
    func = function;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(40, 40);
    glutCreateWindow("Test");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
}

int main(int argc, char** argv) {
    test(argc, argv, func_cos);
//    test(argc, argv, func_cos);
    return 0;
}
