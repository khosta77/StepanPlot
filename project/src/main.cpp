#include "../include/StepanPlot.h"

#include <math.h>

int main(int argc, char** argv) {
    StepanPlot plt(argc, argv);
    {  // Тест 1
        vector<double> X;
        vector<double> Y1, Y2;
        for (double i = -1000.0; i <= 1000.1; i+=0.1) {
            X.push_back(i);
            Y1.push_back(cos(i/20));
            Y2.push_back(sin(i/20));
        }

        plt.plot(X, Y1, "sin and cos", df::Brush(1.0, 0.0, 0.0));
        plt.hold(true);
        plt.plot(X, Y2);
        plt.brush(df::Brush(0.0, 0.0, 1.0));
        plt.hold(false);
        plt.xlim(-50.0, 50.0);
        plt.grid(true);
    }
    {  // Тест 2
        vector<double> X;
        vector<double> Y;
        for (double i = 0.0; i <= 1000.1; i+=0.1) {
            X.push_back(i);
            Y.push_back(exp(i/20));
        }

        plt.plot(X, Y, "exp");
    }
    {  // Тест 3
        vector<double> X;
        vector<double> Y;
        for (double i = -1000.0; i <= 1000.1; i+=0.1) {
            X.push_back(i);
            Y.push_back(cos(i/20) + sin(i/20));
        }

        plt.plot(X, Y);
        plt.xlim(-500.0, 500.0);
        plt.grid(true);
    }
    {  // Тест 4
        vector<double> X;
        vector<double> Y;
        for (double i = 0.0; i <= 1000.1; i+=0.1) {
            X.push_back(i);
            Y.push_back(sqrt(i));
        }

        plt.plot(X, Y, "sqrt", df::Brush(0.0, 0.0, 0.0));
        plt.brush(df::Brush(0.0, 0.0, 1.0));
        plt.xlim(0.0, 50.0);
        plt.ylim(0.0, 100.0);
        plt.grid(true);
    }
    {  // Тест 5
        vector<double> X;
        vector<double> Y;
        for (double i = -100.0; i <= 100.1; i+=0.1) {
            X.push_back(i);
            Y.push_back(cos(i/20) - sin(i/20));
        }

        plt.plot(X, Y, "sin - cos");
    }
    {  // Тест 6
        vector<double> X;
        vector<double> Y;
        for (double i = -1000.0; i <= 1000.1; i+=0.1) {
            X.push_back(i);
            Y.push_back(i*i);
        }

        plt.plot(X, Y, "X^2");
        plt.grid(true);
    }
    return 0;
}
