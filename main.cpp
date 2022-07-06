#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>
#include <map>

using namespace std;

class StepanPlot;

static StepanPlot* currentInstance;

class StepanPlot {
private:
    vector<pair<vector<double>, vector<double>>> df;
    vector<int> window;

public:
    StepanPlot(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(1024, 512);
        glutInitWindowPosition(40, 40);
    }

    void draw() {
        int win = glutGetWindow() - 1;
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
        for (double i = 1; i < StepanPlot::df[win].first.size(); i++) {
            glVertex2d(StepanPlot::df[win].first[i - 1], StepanPlot::df[win].second[i - 1]);
            glVertex2d(StepanPlot::df[win].first[i], StepanPlot::df[win].second[i]);
        }
        glEnd();
        glFlush();
    }

    static void display() {
        currentInstance->draw();
//        int win = glutGetWindow() - 1;
//        glClear(GL_COLOR_BUFFER_BIT);
//        glBegin(GL_LINES);
//        {
//            glColor3f(1,0,0);
//            glVertex2f(-20, 0);
//            glVertex2f(500, 0);
//            glColor3f(0,1, 0);
//            glVertex2f(0, -100);
//            glVertex2f(0, 100);
//        }
//        glEnd();
//        glBegin(GL_POINTS);
//        glColor3f(1, .2, 0);
//        for (double i = 1; i < StepanPlot::df[win].first.size(); i++) {
//            glVertex2d(StepanPlot::df[win].first[i - 1], StepanPlot::df[win].second[i - 1]);
//            glVertex2d(StepanPlot::df[win].first[i], StepanPlot::df[win].second[i]);
//        }
//        glEnd();
//        glFlush();
    }

    void init() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glColor3f(0.6, 1.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-20.0, 500.0, -1.2, 1.2, 0.0, 100.0);
    }

    void plot(vector<double> x, vector<double> y, const char *name) {
        window.push_back(glutCreateWindow(name));
        df.push_back(pair<vector<double>, vector<double>>(x, y));
        currentInstance = this;
        glutDisplayFunc(StepanPlot::display);
        init();
    }

//    void plot(vector<double> x, vector<double> y) {
//        window.push_back(glutCreateWindow("Test"));
//        glutDisplayFunc(StepanPlot::display);
//        init();
//    }

    void call() {
        glutMainLoop();
    }
};

static auto func_cos(double x) {
    return  cos(x/20);
}

static auto func_sin(double x) {
    return  sin(x/20);
}

//static double (*func)(double);
////std::vector<std::function<void(int)>> functions;
//
//vector<pair<vector<double>, vector<double>>> df;
//vector<int> window;
//
//
//void display() {
//    int win = glutGetWindow() - 1;
//    glClear(GL_COLOR_BUFFER_BIT);
//    glBegin(GL_LINES);
//    {
//        glColor3f(1,0,0);
//        glVertex2f(-20, 0);
//        glVertex2f(500, 0);
//        glColor3f(0,1, 0);
//        glVertex2f(0, -100);
//        glVertex2f(0, 100);
//    }
//    glEnd();
//    glBegin(GL_POINTS);
//    glColor3f(1, .2, 0);
//    for (double i = 1; i < df[win].first.size(); i++) {
//        glVertex2d(df[win].first[i - 1], df[win].second[i - 1]);
//        glVertex2d(df[win].first[i], df[win].second[i]);
//    }
//    glEnd();
//    glFlush();
//}
//
//void init() {
//    glClearColor(1.0, 1.0, 1.0, 1.0);
//    glColor3f(0.6, 1.0, 0.0);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-20.0, 500.0, -1.2, 1.2, 0.0, 100.0);
//}
//
//void plot(vector<double> x, vector<double> y, const char *name) {
//    window.push_back(glutCreateWindow(name));
//    df.push_back(pair<vector<double>, vector<double>>(x, y));
//    glutDisplayFunc(display);
//    init();
//}
//
//void test(int argc, char** argv/*, double (*function)(double)*/) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
//    glutInitWindowSize(1024, 512);
//    glutInitWindowPosition(40, 40);
////    int mainWindow = glutCreateWindow("Test");
////    glutDisplayFunc(display);
////    init();
//////    glutMainLoop();
////
////    //дочерние окна
////    int subWindow1 = glutCreateWindow("Test2");
////
////    glutDisplayFunc(display);
////    init();
//
////    glutMainLoop();
//}

int main(int argc, char** argv) {
    vector<double> X;
    vector<double> Y1, Y2;

    for (double i = 0.1; i <= 500; i+=0.1) {
        X.push_back(i);
        Y1.push_back(func_cos(i));
        Y2.push_back(func_sin(i));
    }

//    for (int i = 0; i < X.size(); i++) {
//        cout << i << ") " << X[i] << " " << Y1[i] << " " << Y1[i] << endl;
//    }

    StepanPlot test(argc, argv);
    test.plot(X, Y1, "cos");
    test.plot(X, Y2, "sin");
//    glutMainLoop();
    test.call();
    return 0;
}
