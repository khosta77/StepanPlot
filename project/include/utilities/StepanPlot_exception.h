#ifndef PLOT_TEST_STEPANPLOT_EXCEPTION_H
#define PLOT_TEST_STEPANPLOT_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace stepan_plot {

    namespace MyException {

        class Exception : public std::exception {
            std::string error;

        public:
            Exception(const std::string& msg) : error(msg) {}

            virtual const char* what() const noexcept {
                return error.c_str();
            }

        };

    };
    using namespace MyException;

}

#endif //PLOT_TEST_STEPANPLOT_EXCEPTION_H
