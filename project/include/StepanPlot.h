#ifndef PLOT_TEST_STEPANPLOT_H
#define PLOT_TEST_STEPANPLOT_H

#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>
#include <map>

#include "./utilities/StepanPlot_dop_func.h"
#include "./utilities/StepanPlot_data_frame.h"
#include "./utilities/StepanPlot_exception.h"

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

        df::Frame win_fr;
        df::Position win_pos;

        map<int, vector<df::plot_frame>> plt;

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
                glColor3f(plt[win][st].br.r, plt[win][st].br.g, plt[win][st].br.b);
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
            df::Ortho ort = get_this_ortho(plt[glutGetWindow()]);
            glColor3f(0.9, 0.9, 0.9);  // Вынести в константу это
            glBegin(GL_LINES);
            for (double dy = ort.b, shag = (ort.t - ort.b) / 10; dy <= ort.t; dy += shag) {  // Вынести в константу коэффицент с клетками
                glVertex2d(ort.l, dy);
                glVertex2d(ort.r, dy);
            }
            for (double dx = ort.l, shag = (ort.r - ort.l) / 10; dx <= ort.r; dx += shag) {
                glVertex2d(dx, ort.b);
                glVertex2d(dx, ort.t);
            }

            glEnd();
        }

        static void display() {
            currentInstance->draw();
        }

        void init_display_mode() {
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
            glutInitWindowSize(win_fr.w, win_fr.h);
            glutInitWindowPosition(win_pos.X, win_pos.Y);
            win_pos.next(win_fr);
        }

        void init() {
            glClearColor(1.0, 1.0, 1.0, 1.0);

            df::Ortho ort = get_this_ortho(plt[glutGetWindow()]);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(ort.l, ort.r, ort.b, ort.t, ort.n, ort.f);

//        pixels = (unsigned char*)malloc(FORMAT_NBYTES * WIDTH * HEIGHT);  // Выделение пиксилей
        }

        df::Ortho get_this_ortho(vector<df::plot_frame> vecPlt) {
            vector<double> OX_left, OX_right, OX_bottom, OX_top;
            for (size_t i = 0; i < vecPlt.size(); i++) {
                OX_left.push_back(vecPlt[i].ort_XOY.l);
                OX_right.push_back(vecPlt[i].ort_XOY.r);
                OX_bottom.push_back(vecPlt[i].ort_XOY.b);
                OX_top.push_back(vecPlt[i].ort_XOY.t);
            }

            df::Ortho ort(af::min_elem(OX_left), af::max_elem(OX_right),
                          af::min_elem(OX_bottom), af::max_elem(OX_top));

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

            df::Ortho ort(x, y);
            df::plot_frame pl(pair<vector<double>, vector<double>>(x, y), ort);

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

        int initDisplay(df::plot_frame pl, const char *plotName) {
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
