#ifndef PLOT_TEST_STEPANPLOT_H
#define PLOT_TEST_STEPANPLOT_H

#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>
#include <map>

#include "./utilities/StepanPlot_dop_func.h"

using namespace std;

// TODO: на 09.08
//  1) lim-ы
//  2) Возможность кисти, для пользователя
//  3) Разбить класс на под блоки
//  4) Начать коментировать, вынести в отдельное пространство имен

// TODO: 5) Попробовать все сделать статическими, а не классом. Правда за чем?

/* Нельзя просто так взять и сделать вывод текста */

namespace stepan_plot {

    class StepanPlot;

    static StepanPlot *currentInstance;

    class StepanPlot {
    private:
        bool hold_status = false;
        bool first_plot = false;

        int currentWindow;

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
                r = af::rand_factor(1.0, 0.0);
                b = af::rand_factor(1.0, 0.0);
                g = af::rand_factor(1.0, 0.0);
            }
        };

        struct plotInfo {
            string name;
            pair <vector<double>, vector<double>> XOY;
            ortho ort_XOY;
            brush pb;
            bool grid_status = false;
        };

        map<int, vector<plotInfo>> plt;

    public:
        StepanPlot(int argc, char **argv) {
            glutInit(&argc, argv);
            glutSetOption(
                    GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_CONTINUE_EXECUTION);
            currentWindow = 0;
        }

        void draw() {
            if (plt.empty()) {
                return;
            }

            int win = glutGetWindow();
            glClear(GL_COLOR_BUFFER_BIT);

            if (plt[win][0].grid_status) {
                grid();
            }

            glBegin(GL_POINTS);
            for (size_t st = 0; st < plt[win].size(); st++) {
                glColor3f(plt[win][st].pb.r, plt[win][st].pb.g, plt[win][st].pb.b);
                glLineWidth(0);
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
            glColor3f(0.9, 0.9, 0.9);
            glBegin(GL_LINES);
            for (double dy = ort.bottom, shag = (ort.top - ort.bottom) / 10; dy <= ort.top; dy += shag) {
                glVertex2d(ort.left, dy);
                glVertex2d(ort.right, dy);
            }
            for (double dx = ort.left, shag = (ort.right - ort.left) / 10; dx <= ort.right; dx += shag) {
                glVertex2d(dx, ort.bottom);
                glVertex2d(dx, ort.top);
            }

            glEnd();
        }

        static void display() {
            currentInstance->draw();
        }

        void init_display_mode() {
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
            glutInitWindowSize(fr.WIDTH, fr.HEIGHT);
            glutInitWindowPosition(pos.X, pos.Y);
            pos.X += fr.WIDTH; // Теперь окна не будут друг на друга накладываться, но что делать если их много \
        Надо будет получать размер монитора ипроверять вмещаются ли в нем все графики
        }

        void init() {
            glClearColor(1.0, 1.0, 1.0, 1.0);

            ortho ort = get_this_ortho(plt[glutGetWindow()]);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(ort.left, ort.right, ort.bottom, ort.top, 0.0, 100.0);

//        pixels = (unsigned char*)malloc(FORMAT_NBYTES * WIDTH * HEIGHT);  // Выделение пиксилей
        }

        ortho get_this_ortho(vector<plotInfo> vecPlt) {
            vector<double> OX_left, OX_right, OX_bottom, OX_top;
            for (size_t i = 0; i < vecPlt.size(); i++) {
                OX_left.push_back(vecPlt[i].ort_XOY.left);
                OX_right.push_back(vecPlt[i].ort_XOY.right);
                OX_bottom.push_back(vecPlt[i].ort_XOY.bottom);
                OX_top.push_back(vecPlt[i].ort_XOY.top);
            }

            ortho ort = {
                    .left = af::min_elem(OX_left),
                    .right = af::max_elem(OX_right),
                    .bottom = af::min_elem(OX_bottom),
                    .top = af::max_elem(OX_top)
            };

            return ort;
        }

        void hold(bool status) {
            hold_status = status;
        }

        void grid(bool status) {
            if (currentWindow == 0) {
                return;
            }

            if (plt[currentWindow].empty()) {
                return;
            }

            plt[currentWindow][0].grid_status = status;
        }

        void plot(vector<double> x, vector<double> y, const char *plotName) {
            if (x.empty() || y.empty()) {
                return;  // Вывести ошибку
            }

            ortho ort_OXY = {  // Переделать - внести в конструктор
                    .left = af::min_elem(x),
                    .right = af::max_elem(x),
                    .bottom = af::min_elem(y),
                    .top = af::max_elem(y)
            };

            plotInfo pl = {
                    .name = plotName,
                    .XOY = pair<vector<double>, vector<double>>(x, y),
                    .ort_XOY = ort_OXY,
            };

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
            unsigned int cW = glutCreateWindow(plotName);
            plt[cW].push_back(pl);
            currentInstance = this;
            glutDisplayFunc(StepanPlot::display);
            init();
            return cW;
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
            *img_pixels = (GLubyte *) realloc(*img_pixels,
                                              format_nchannels * sizeof(GLubyte) * width * height);
            glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, *img_pixels);
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                    cur = format_nchannels * ((height - i - 1) * width + j);
                    fprintf(f, "%3d %3d %3d ", (*img_pixels)[cur], (*img_pixels)[cur + 1],
                            (*img_pixels)[cur + 2]);
                }
                fprintf(f, "\n");
            }
            fclose(f);
        }

//------------------------------------------------------------------------------------------------------------
        void call() {
            glutMainLoop();
        }

        ~StepanPlot() {
            glutMainLoop();
        }
    };
};

using namespace stepan_plot;

#endif // PLOT_TEST_STEPANPLOT_H
