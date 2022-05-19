//
// Created by Akman on 2022/3/22.
//
#include "logarithm.h"

#include <cmath>
#include <exception>

namespace softmax {

namespace module {

const half Logarithm::ln2 = half{0.6931471805599453};

LinearFunctionParameters Logarithm::GetLinearLog2Params(int n, bool print_params) {
    LinearFunctionParameters params;
    for (int i = 0; i < n; i++) {
        double x1 = (i + 0.0) / n + 1.0;
        double x2 = (i + 1.0) / n + 1.0;
        double k = (log2(x2) - log2(x1)) / (x2 - x1);
        double b1 = log2(x1) - k * x1;

        double x_ = 1.0 / log(2) / k;
        double b2 = log2(x_) - k * x_;

        double b = (b1 + b2) / 2.0;

        if (print_params) {
            std::cout << "y = " << k << " * x + " << b << ", [" << x1 << ", " << x2 << ")"
                      << std::endl;
        }
        params.emplace_back(LinearFunctionParameter{
            {"x1", half{x1}}, {"x2", half{x2}}, {"k", half{k}}, {"b", half{b}}});
    }
    return std::move(params);
}

half Logarithm::LinearLog2(const half &x) {
    for (auto &param : params_) {
        if (param["x1"] <= x && x < param["x2"]) {
            return x * param["k"] + param["b"];
        }
    }
    return half{0.0};
}

half Logarithm::Calculate(const half &input) {
    if (input <= half(0.0)) {
        throw std::out_of_range{"log input is negative"};
    }

    int exp_num = ((input.GetBits() >> 10) & 0x1f) - 15;
    half w{(float)exp_num};
    half k((input.GetBits() & 0x3ff) | 0x400, 0xf, 0x0);

    return (LinearLog2(k) + w) * ln2;
}

}  // namespace module

}  // namespace softmax
