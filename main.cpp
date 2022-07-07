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


//    GLubyte *pixels = NULL;
//    const GLenum FORMAT = GL_RGBA;
    const GLuint FORMAT_NBYTES = 4;
    const unsigned int HEIGHT = 512;
    const unsigned int WIDTH = 1024;
public:
    StepanPlot(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(WIDTH, HEIGHT);
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
        screenshot_ppm("tmp", (uint32_t)WIDTH, (uint32_t)HEIGHT, &pixels);  // \
        вводим команду чтобы сделать скриншот
    }

    static void display() {
        currentInstance->draw();
    }

    void init() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glColor3f(0.6, 1.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-20.0, 500.0, -1.2, 1.2, 0.0, 100.0);
        pixels = (unsigned char*)malloc(FORMAT_NBYTES * WIDTH * HEIGHT);  // ???
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
//------------------------------------------------------------------------------------------------------------
    GLubyte *pixels = nullptr;  /**< Необходимо для создания файлов изображений */


    /**  \brief - Делает скриншот окна, Последнегео открытого окна.
     * \param - имя файла
     * \param - ширина
     * \param - высота
     * \param - ???
     * */
    void screenshot_ppm(
            const char *filename,
            const uint32_t width,
            const uint32_t height,
            GLubyte **img_pixels) {
        size_t i, j, cur;
        const size_t format_nchannels = 3;
        FILE *f = fopen(filename, "w");
        fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
        *img_pixels = (GLubyte*)realloc(*img_pixels, format_nchannels * sizeof(GLubyte) * width * height);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, *img_pixels);
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                cur = format_nchannels * ((height - i - 1) * width + j);
                fprintf(f, "%3d %3d %3d ", (*img_pixels)[cur], (*img_pixels)[cur + 1], (*img_pixels)[cur + 2]);
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }
//------------------------------------------------------------------------------------------------------------
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
    test.call();
    return 0;
}
