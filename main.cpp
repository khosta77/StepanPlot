#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>
#include <map>

// TODO: на 08.08
//  2) Красивая отрисовки сетки
//  3) Возможность писать текст на графике
//  4) Рисовать оси, если они попадают
//  5) ...

using namespace std;

static double DoubleRand(double _max, double _min) {
    return _min + double(rand()) / RAND_MAX * (_max - _min);
}

class StepanPlot;

static StepanPlot* currentInstance;

class StepanPlot {
private:
    bool hold_status = false;
    bool first_plot = false;
    bool grid_status = true;
    vector<pair<vector<double>, vector<double>>> df;
    vector<int> window;
    const GLuint FORMAT_NBYTES = 4;

    struct frame {  // Рамка
        unsigned int HEIGHT = 512;
        unsigned int WIDTH = 512;
    } fr;

    struct position {  // Расположение при создании нового окна
        unsigned int X = 0;
        unsigned int Y = 0;
    } pos;

    struct ortho {  // Характеристики окна в котором стоим график
        double left;
        double right;
        double bottom;
        double top;
        double near = 0.0;
        double far = 100.0;
    };

    struct brush {  // Кисть
        double r;
        double b;
        double g;

        brush() {
            r = DoubleRand(1.0, 0.0);
            b = DoubleRand(1.0, 0.0);
            g = DoubleRand(1.0, 0.0);
        }
    };

//    vector<ortho> ort;

    struct plotInfo {
        string name;
        pair<vector<double>, vector<double>> XOY;
        ortho ort_XOY;
        brush pb;
    };

    map<int, vector<plotInfo>> plt;

private:
    double max(vector<double> O) {
        if (O.empty()) {
            return 0;
        }

        double max = O[0];
        for (size_t i = 0; i < O.size(); i++) {
            if (O[i] > max) {
                max = O[i];
            }
        }

        return max;
    }

    double min(vector<double> O) {
        if (O.empty()) {
            return 0;
        }

        double min = O[0];
        for (size_t i = 0; i < O.size(); i++) {
            if (O[i] < min) {
                min = O[i];
            }
        }

        return min;
    }

public:
    StepanPlot(int argc, char** argv) {
        glutInit(&argc, argv);
        glutSetOption(
                GLUT_ACTION_ON_WINDOW_CLOSE,
                GLUT_ACTION_CONTINUE_EXECUTION);
    }

    void draw() {
        int win = glutGetWindow();
        glClear(GL_COLOR_BUFFER_BIT);
//        grid_status ? grid() : 1;
        grid();
//        {  // Сетка
//            glColor3f(0, 0, 0);
//            glBegin(GL_POINTS);
//            for (double ax = -100.0; ax < 100.0; ax += 0.25) {
//                for (double bx = -100.0; bx < 100.0; bx += 0.25)
//                {
//                    glVertex2d(ax, bx);
//                }
//            }
//            glEnd();
//        }
//        glBegin(GL_LINES);  // Оси
//        {
//            glColor3f(1,0,0);
//            glVertex2f(-20, 0);
//            glVertex2f(500, 0);
//            glColor3f(0,1, 0);
//            glVertex2f(0, -100);
//            glVertex2f(0, 100);
//        }
//        glEnd();
        glBegin(GL_POINTS);
        if (plt.empty()) {
            return;
        }

        for (size_t st = 0; st < plt[win].size(); st++) {
            glColor3f(plt[win][st].pb.r, plt[win][st].pb.g, plt[win][st].pb.b);
            for (double i = 1; i < plt[win][st].XOY.first.size(); i++) {
                glVertex2d(plt[win][st].XOY.first[i - 1],
                           plt[win][st].XOY.second[i - 1]);
                glVertex2d(plt[win][st].XOY.first[i],
                           plt[win][st].XOY.second[i]);
            }
        }

        glEnd();
        glFlush();
//        screenshot_ppm("tmp", (uint32_t)WIDTH, (uint32_t)HEIGHT, &pixels);  // \
        вводим команду чтобы сделать скриншот
    }

    void grid() {
        ortho ort = get_this_ortho(plt[glutGetWindow()]);
        cout << ort.left << " " << ort.right  << " " <<  ort.bottom  << " " << ort.top << "|" << int(ort.right - ort.left)/10 << endl;

        glColor3f(0, 0, 0);
        glBegin(GL_POINTS);

        double shag_x = 0.25;  // (ort.right - ort.left)/5;

        for (double dx = ort.bottom, shag = (ort.top - ort.left)/10; dx < ort.top; dx += shag) {
            glVertex2d(ort.left, dx);
            glVertex2d(ort.right, dx);
        }
//        for (double ax = ort.bottom; ax < ort.top; ax += shag_x) {
//            for (double bx = ort.left; bx < ort.right; bx += shag_x) {
//                glVertex2d(bx, ax);
//            }
//        }
//        for (double ax = ort.top; ax > ort.bottom; ax -= shag_x) {
//            for (double bx = ort.right; bx > ort.left; bx -= shag_x) {
//                glVertex2d(ax, bx);
//            }
//        }
        glEnd();

//        glColor3f(0, 0, 0);
//        glBegin(GL_POINTS);
//
//        double shag_x = 0.25;  // (ort.right - ort.left)/5;
//        for (double ax = ort.bottom; ax < ort.top; ax += shag_x) {
//            for (double bx = ort.bottom; bx < ort.top; bx += shag_x) {
//                glVertex2d(ax, bx);
//            }
//        }
//
//        glEnd();
    }

    static void display() {
        currentInstance->draw();
    }

    void init_display_mode() {
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(fr.WIDTH, fr.HEIGHT);
        glutInitWindowPosition(pos.X, pos.Y);
    }

    void init() {
        glClearColor(1.0, 1.0, 1.0, 1.0);

        // Выделить это в отдельную переменную
        vector<double> OX_left, OX_right, OX_bottom, OX_top;
        for (size_t i = 0; i < plt[glutGetWindow()].size(); i++) {
            OX_left.push_back(plt[glutGetWindow()][i].ort_XOY.left);
            OX_right.push_back(plt[glutGetWindow()][i].ort_XOY.right);
            OX_bottom.push_back(plt[glutGetWindow()][i].ort_XOY.bottom);
            OX_top.push_back(plt[glutGetWindow()][i].ort_XOY.top);
        }

        ortho ort = {
                .left = min(OX_left),
                .right = max(OX_right),
                .bottom = min(OX_bottom),
                .top = max(OX_top)
        };

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        cout << ort.left << " " << ort.right  << " " <<  ort.bottom  << " " << ort.top << endl;
        glOrtho(ort.left, ort.right, ort.bottom, ort.top, 0.0, 100.0);

//        pixels = (unsigned char*)malloc(FORMAT_NBYTES * WIDTH * HEIGHT);  // Выделение пиксилей
    }

    ortho get_this_ortho(vector<plotInfo> vecPlt) {
        // Выделить это в отдельную переменную
        vector<double> OX_left, OX_right, OX_bottom, OX_top;
        for (size_t i = 0; i < vecPlt.size(); i++) {
            OX_left.push_back(vecPlt[i].ort_XOY.left);
            OX_right.push_back(vecPlt[i].ort_XOY.right);
            OX_bottom.push_back(vecPlt[i].ort_XOY.bottom);
            OX_top.push_back(vecPlt[i].ort_XOY.top);
        }

        ortho ort = {
                .left = min(OX_left),
                .right = max(OX_right),
                .bottom = min(OX_bottom),
                .top = max(OX_top)
        };

        return ort;
    }

    void hold(bool status) {
        hold_status = status;
    }

    void grid(bool status) {
        grid_status = status;
    }

    void plot(vector<double> x, vector<double> y, const char *plotName) {
        if (x.empty() || y.empty()) {
            return;  // Вывести ошибку
        }

        ortho ort_OXY = {
                .left = min(x),
                .right = max(x),
                .bottom = min(y),
                .top = max(y)
        };

        plotInfo pl = {
                .name = plotName,
                .XOY = pair<vector<double>, vector<double>>(x, y),
                .ort_XOY = ort_OXY,
        };

        static unsigned int currentWindow;
        if (!hold_status) {
            currentWindow = initDisplay(pl, plotName);
        } else {
            if (plt.empty()) {
                currentWindow = initDisplay(pl, plotName);
            } else {
                plt[currentWindow].push_back(pl);
                init();
            }
        }
    }

    int initDisplay(plotInfo pl, const char *plotName) {
        init_display_mode();
        unsigned int currentWindow = glutCreateWindow(plotName);
        plt[currentWindow].push_back(pl);
        currentInstance = this;
        glutDisplayFunc(StepanPlot::display);
        init();
        return currentWindow;
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

static auto func_exp(double x) {
    return  exp(x/20);
}

int main(int argc, char** argv) {
    vector<double> X;
    vector<double> Y1, Y2, Y3;

    for (double i = 0.0; i <= 500.1; i+=0.1) {
        X.push_back(i);
        Y1.push_back(func_cos(i));
        Y2.push_back(func_sin(i));
        Y3.push_back(func_exp(i));
    }

//    for (int i = 0; i < X.size(); i++) {
//        cout << i << ") " << X[i] << " " << Y1[i] << " " << Y1[i] << endl;
//    }

    StepanPlot test(argc, argv);
    test.plot(X, Y1, "cos");
//    test.hold(true);
//    test.plot(X, Y2, "sin");
////    test.plot(X, Y1, "cos");
//    test.hold(true);
//    test.plot(X, Y3, "exp");
    test.call();
    return 0;
}
