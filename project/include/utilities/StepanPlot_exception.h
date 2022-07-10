#ifndef PLOT_TEST_STEPANPLOT_EXCEPTION_H
#define PLOT_TEST_STEPANPLOT_EXCEPTION_H

#include <GL/freeglut.h>

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace stepan_plot {

    namespace MyException {

        /** \class - Вывод ошибки
         * */
        class Exception : public std::exception {

            std::string error;  /* Текст с ошибкой */

        public:
            Exception(const std::string& msg) : error(msg) {}

            virtual const char* what() const noexcept {
                return error.c_str();
            }

        };

    };  // MyException

    using namespace MyException;

};  // stepan_plot

#endif //PLOT_TEST_STEPANPLOT_EXCEPTION_H
