#ifndef PLOT_TEST_STEPANPLOT_DATA_FRAME_H
#define PLOT_TEST_STEPANPLOT_DATA_FRAME_H

#include "StepanPlot_dop_func.h"

namespace stepan_plot {

    namespace df {
//------------------------------------------------------------------------------------------------------------
        /**\brief - класс рамка
         * */
        class Frame {
        public:
            unsigned int h = 512;   /* Высота рамки */
            unsigned int w = 512;   /* Ширина рамки */

            /** \brief Инициализация параметров рамки по умолчанию
             * */
            Frame() {}

            /** \brief Инициализация параметров рамки
             * \param height высота
             * \param width широта
             */
            Frame(unsigned int height, unsigned int width) : h(height), w(width) {}
        };
//------------------------------------------------------------------------------------------------------------
        /**\brief - класс позиции нового окна
         * */
        class Position {
        private:
            /* Размеры дисплея, по умолчанию */
            unsigned int display_height = 1080;
            unsigned int display_width = 1920;

        public:
            unsigned int X = 0;  /* Положение по Х */
            unsigned int Y = 0;  /* Положение по У */

            /** \brief Инициализация параметров рамки по умолчанию
             * */
            Position() {}

            /** \brief Инициализация параметров рамки
             * \param x смещение по х
             * \param y смещение по y
             */
            Position(unsigned int x, unsigned int y) : X(x), Y(y) {}

            /** \brief Переход к следующему положению
             * \param fr рамка
             */
            void next(Frame fr) {
                if ((X + fr.w) <= display_width) {
                    X += fr.w;
                    return;
                }

                if ((Y + fr.h) <= display_height) {
                    Y += fr.h;
                    X = 0;
                    return;
                }

                X = 0;
                Y = 0;
            }
        };
//------------------------------------------------------------------------------------------------------------
        /** \brief Характеристики окна в котором строим график
         * */
        class Ortho {
        public:
            double l;
            double r;
            double b;
            double t;
            double n = 0.0;
            double f = 100.0;

            Ortho() {}

            Ortho(double left, double right, double bottom, double top, double near = 0.0, double far = 100.0)
               : l(left), r(right), b(bottom), t(top), n(near), f(far) {}

            Ortho(std::vector<double> X, std::vector<double> Y) {
                try {
                    if (X.empty() || Y.empty()) {
                        throw Exception("---> X or Y empty\n");
                    }

                    if (X.size() != Y.size()) {
                        throw Exception("---> X != Y\n");
                    }
                } catch(Exception& e) {
                    std::cout << e.what() << std::endl;
                }

                l = af::min_elem(X);
                r = af::max_elem(X);
                b = af::min_elem(Y);
                t = af::max_elem(Y);
            }
        };
//------------------------------------------------------------------------------------------------------------
        class Brush {
        public:
            double r;
            double b;
            double g;

            Brush() {
                r = af::rand_factor(1.0, 0.0);
                b = af::rand_factor(1.0, 0.0);
                g = af::rand_factor(1.0, 0.0);
            }

            Brush(double red, double blue, double green) : r(red), b(blue), g(green) {}
        };
//------------------------------------------------------------------------------------------------------------
        class plot_frame {
        public:
//            std::string name;  |* Имя графика *|
            std::pair<std::vector<double>, std::vector<double>> XOY;  /* Оси Х и У */
            Ortho ort_XOY;  /* Рамка для каждого графика */
            Brush br;  /* Цвет линии */
            bool grid_status = false;  /* Сетка графика */

            plot_frame() {}

            plot_frame(std::pair<std::vector<double>, std::vector<double>> xoy, Ortho ort, Brush pb = Brush())
               : XOY(xoy), ort_XOY(ort), br(pb) {}
        };
//------------------------------------------------------------------------------------------------------------
    };

};

#endif // PLOT_TEST_STEPANPLOT_DATA_FRAME_H
