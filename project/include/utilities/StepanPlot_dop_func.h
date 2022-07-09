#ifndef PLOT_TEST_STEPANPLOT_DOP_FUNC_H
#define PLOT_TEST_STEPANPLOT_DOP_FUNC_H

#include "StepanPlot_exception.h"

namespace stepan_plot {

    namespace additional_functions {

        class additional_func {
        public:
//------------------------------------------------------------------------------------------------------------
            /** \brief - Необходима для задания случайного цвета линии графика
             * \param max
             * \param min
             * */
            static double rand_factor(double max, double min) {
                return min + double(rand()) / RAND_MAX * (max - min);
            }
//------------------------------------------------------------------------------------------------------------
            /** \brief - Возращает максимальный элемент вектора
             * \param O - ось
             * */
            static double max_elem(std::vector<double> O) {
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
//------------------------------------------------------------------------------------------------------------
            /** \brief - Возращает минимальный элемент вектора
             * \param O - Ось
             * */
            static double min_elem(std::vector<double> O) {
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

        };

    };

    using af = additional_functions::additional_func;

};

#endif //PLOT_TEST_STEPANPLOT_DOP_FUNC_H
