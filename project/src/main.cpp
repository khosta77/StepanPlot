#include "../include/StepanPlot.h"

static auto func_cos(double x) {
    return  x*x;  // cos(x/20);
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

    for (double i = -100.0; i <= 100.1; i+=0.1) {
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
    test.grid(true);
//    test.hold(true);
    test.plot(X, Y2, "sin");
//    test.grid(false);
////    test.plot(X, Y1, "cos");
//    test.hold(true);
//    test.plot(X, Y3, "exp");
//    test.call();
    return 0;
}
