#ifndef PLOT_TEST_STEPANPLOT_H
#define PLOT_TEST_STEPANPLOT_H

//#include <GL/freeglut.h>
//#include <iostream>
//#include <functional>
//#include <vector>
//#include <map>
//#include <string>

#include "./utilities/StepanPlot_dop_func.h"
#include "./utilities/StepanPlot_data_frame.h"
#include "./utilities/StepanPlot_exception.h"

using namespace std;

// TODO: на 10.08
//  3) Добавить комментарии ко всем функциям/методам
//  4) Начать собирать проект (добавить проверки на подключенные библиотеки, блоки try, catch и т д)

/* Нельзя просто так взять и сделать вывод текста */

namespace stepan_plot {

    class StepanPlot;

    static StepanPlot *currentInstance;

    class StepanPlot {
    private:
        unsigned int plot_size = 0;

        bool hold_status = false;
        bool first_plot = false;

        int currentWindow = 0;

        df::Frame win_fr;
        df::Position win_pos;

        map<int, vector<df::plot_frame>> plt;

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
        }

        void grid() {
            const unsigned int number_of_cells = 10;

            df::Ortho ort = df::get_ortho(plt[glutGetWindow()]);

            glColor3f(0.9, 0.9, 0.9);  // Вынести в константу это
            glBegin(GL_LINES);
            for (double dy = ort.b, shag = (ort.t - ort.b) / number_of_cells; dy <= ort.t; dy += shag) {
                glVertex2d(ort.l, dy);
                glVertex2d(ort.r, dy);
            }
            for (double dx = ort.l, shag = (ort.r - ort.l) / number_of_cells; dx <= ort.r; dx += shag) {
                glVertex2d(dx, ort.b);
                glVertex2d(dx, ort.t);
            }

            glEnd();
        }

        void init_display_mode() {
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
            glutInitWindowSize(win_fr.w, win_fr.h);
            glutInitWindowPosition(win_pos.X, win_pos.Y);
            win_pos.next(win_fr);
        }

        void init() {
            glClearColor(1.0, 1.0, 1.0, 1.0);  // Вынести в константы

            df::Ortho ort = df::get_ortho(plt[glutGetWindow()]);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(ort.l, ort.r, ort.b, ort.t, ort.n, ort.f);
        }

        static void display() {
            currentInstance->draw();
        }

        int initDisplay(df::plot_frame pl, std::string plotName) {
            init_display_mode();
            unsigned int cW = glutCreateWindow(plotName.c_str());
            plt[cW].push_back(pl);
            currentInstance = this;
            glutDisplayFunc(StepanPlot::display);
            return cW;
        }

    public:
        StepanPlot() {
            try {
                throw Exception("---> В конструктор необходимо передать argc и argv\n");
            } catch(Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

        StepanPlot(int argc, char **argv) {
            glutInit(&argc, argv);
            glutSetOption(
                    GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_CONTINUE_EXECUTION);
        }

        void plot(std::vector<double> x, std::vector<double> y, std::string plotName, df::Brush br) {
            df::Ortho ort(x, y);
            df::plot_frame pl(pair<vector<double>, vector<double>>(x, y), ort, br);

            if (!hold_status) {
                currentWindow = initDisplay(pl, plotName);
            } else {
                if (plt.empty()) {
                    currentWindow = initDisplay(pl, plotName);
                } else {
                    plt[currentWindow].push_back(pl);
                }
            }

            init();
        }

        void plot(std::vector<double> x, std::vector<double> y, std::string plotName) {
            plot(x, y, plotName, df::Brush());
        }

        void plot(std::vector<double> x, std::vector<double> y) {
            plot(x, y, "plot " + std::to_string(++plot_size), df::Brush());
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

        void brush(df::Brush br) {
            if (currentWindow == 0) {
                return;
            }

            if (plt[currentWindow].empty()) {
                return;
            }

            plt[currentWindow][plt[currentWindow].size() - 1].br = br;
        }

        void xlim(double left, double right) {
            try {
                if (currentWindow == 0) {
                    throw Exception("---> Не существует окна\n");
                }

                if(plt[currentWindow].empty()) {
                    throw Exception("---> ??? график не создан\n");
                }

                plt[currentWindow][0].ul.x_lim(left, right);
                init();
            } catch(Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

        void ylim(double bottom, double top) {
            try {
                if (currentWindow == 0) {
                    throw Exception("---> Не существует окна\n");
                }

                if(plt[currentWindow].empty()) {
                    throw Exception("---> ??? график не создан\n");
                }

                plt[currentWindow][0].ul.y_lim(bottom, top);
                init();
            } catch(Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

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
