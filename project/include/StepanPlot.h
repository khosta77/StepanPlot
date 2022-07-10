#ifndef PLOT_TEST_STEPANPLOT_H
#define PLOT_TEST_STEPANPLOT_H

#include "./utilities/StepanPlot_dop_func.h"
#include "./utilities/StepanPlot_data_frame.h"
#include "./utilities/StepanPlot_exception.h"

namespace stepan_plot {

    using namespace std;

    class StepanPlot;

    static StepanPlot *currentInstance;  /* Статический объект класса необходим для вызова \
                                                                                   метода draw() из класса */
    /** \class - Для построения графиков
     * */
    class StepanPlot {
    private:
        unsigned int plot_size = 0;  /* Колличество не подписанных графиков */

        bool hold_status = false;  /* Статус отрисовки на одном окне графиков */
        bool first_plot = false;  /* В некоторых моментах важно уточнить есть уже графики или нет */

        int currentWindow = 0;  /* Текущее окно, используется если добовляем новые графики на одно окно */

        df::Frame win_fr;  /* Характеристики рамки, идейно пользователь не должен ее редактировать */
        df::Position win_pos;  /* Позиция новых окон */

        std::map<int, vector<df::plot_frame>> plt;  /* Окно с графиком */
//------------------------------------------------------------------------------------------------------------
        /** \brief - метод отрисовки графика/ов в окне
         * */
        void draw() {
            if (plt.empty()) {
                return;
            }

            int win = glutGetWindow();
            glClear(GL_COLOR_BUFFER_BIT);

            if (plt[win][0].grid_status) {  // Информация о отрисовывки хранится в 1-ой ячейки
                grid();
            }

            glBegin(GL_POINTS);
            for (size_t st = 0; st < plt[win].size(); st++) {
                glColor3f(plt[win][st].br.r, plt[win][st].br.g, plt[win][st].br.b);

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
//------------------------------------------------------------------------------------------------------------
        /** \brief - метод отрисовки сетки, вызывается из метода draw()
         * */
        void grid() {
            const unsigned int number_of_cells = 10;  // Колличество клеток в одной строке

            df::Ortho ort = df::get_ortho(plt[glutGetWindow()]);

            glColor3f(df::grid_color.r, df::grid_color.g, df::grid_color.b);
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
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод инициализации размеров области рисования в окне, вызывается когда изменяются \
         * характеристики области в которой отрисовывается график
         * */
        void init() {
            glClearColor(df::background.r, df::background.g, df::background.b, df::background.a);

            df::Ortho ort = df::get_ortho(plt[glutGetWindow()]);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(ort.l, ort.r, ort.b, ort.t, ort.n, ort.f);
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод нужен для того, чтобы вызвать метод draw(), который является членом класса
         * */
        static void display() {
            currentInstance->draw();
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод создает новое окно
         * \param pl - Объект графика, его характеристики(см. класс \plot_frame)
         * \param plotName - имя графика
         * */
        int initDisplay(df::plot_frame pl, std::string plotName) {
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
            glutInitWindowSize(win_fr.w, win_fr.h);
            glutInitWindowPosition(win_pos.X, win_pos.Y);
            win_pos.next(win_fr);
            unsigned int cW = glutCreateWindow(plotName.c_str());
            plt[cW].push_back(pl);
            currentInstance = this;
            glutDisplayFunc(StepanPlot::display);
            return cW;
        }
//------------------------------------------------------------------------------------------------------------
    public:
        /** \brief - Конструктор класса StepanPlot, если в него ничего не переданно \
         * Прекратит выполнение программы
         * */
        StepanPlot() {
            try {
                throw Exception("---> В конструктор необходимо передать argc и argv\n");
            } catch(Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

        /** \brief - Конструктор класс, принимающий параметры
         * \param argc
         * \param argv
         * ! Если не пердать в него параметры ничего работать не будет
         * */
        StepanPlot(int argc, char **argv) {
            glutInit(&argc, argv);
            glutSetOption(
                    GLUT_ACTION_ON_WINDOW_CLOSE,  // Чтобы можно было закрывать окна
                    GLUT_ACTION_CONTINUE_EXECUTION);
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод отрисовки графика 1
         * \param x - ось Х
         * \param y - ось У
         * \param plotName - имя графика
         * \param br - кисть
         * */
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

        /** \brief - Метод отрисовки графика 2
         * \param x - ось Х
         * \param y - ось У
         * \param plotName - имя графика
         * Кисть случайная
         * */
        void plot(std::vector<double> x, std::vector<double> y, std::string plotName) {
            plot(x, y, plotName, df::Brush());
        }

        /** \brief - Метод отрисовки графика 3
         * \param x - ось Х
         * \param y - ось У
         * Имя графика случайное
         * Кисть случайная
         * */
        void plot(std::vector<double> x, std::vector<double> y) {
            plot(x, y, "plot " + std::to_string(++plot_size), df::Brush());
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод отвечающий за то будет ли в окне отрисован 1 или более график
         * \param status - если true, то > 1 , если false только 1
         * */
        void hold(bool status) {
            hold_status = status;
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод отрисовки сетки
         * \param status - отрисовывать сетку или нет
         * */
        void grid(bool status) {
            try {
                if (currentWindow == 0) {
                    throw Exception("---> Не существует окна\n");
                }

                if(plt[currentWindow].empty()) {
                    throw Exception("---> ??? график не создан\n");
                }

                plt[currentWindow][0].grid_status = status;
            } catch(Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод изменения кисти
         * \param br - кисть
         * */
        void brush(df::Brush br) {
            try {
                if (currentWindow == 0) {
                    throw Exception("---> Не существует окна\n");
                }

                if(plt[currentWindow].empty()) {
                    throw Exception("---> ??? график не создан\n");
                }

                plt[currentWindow][plt[currentWindow].size() - 1].br = br;
            } catch(Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод установки приделов по оси Х
         * \param left - слева
         * \param right - справа
         * */
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

        /** \brief - Метод установки приделов по оси У
         * \param bottom - снизу
         * \param top - сверху
         * */
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
//------------------------------------------------------------------------------------------------------------
        /** \brief - Метод вызво отрисовки ВСЕХ графиков
         * ! Примечание: код после не будет исполнен
         * */
        void call() {
            glutMainLoop();
        }

        /** \brief  - Деструктор, аналогичен методу call()
         * */
        ~StepanPlot() {
            glutMainLoop();
        }

    };  // StepanPlot

};  // stepan_plot

using namespace stepan_plot;

#endif // PLOT_TEST_STEPANPLOT_H
