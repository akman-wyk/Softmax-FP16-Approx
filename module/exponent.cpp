//
// Created by Akman on 2022/3/21.
//
#include "exponent.h"

#include <cmath>
#include <iostream>
#include <bitset>

namespace softmax {

namespace module {

const half Exponent::min_input = half(-9.70405981103949);
const half Exponent::max_input = half(11.089866488461016);
const half Exponent::log2e = half(1.4426950408889634);

half Exponent::Calculate(const half& input) {
    if (input <= min_input) {
        return half{0.0};
    } else if (input >= max_input) {
        return std::numeric_limits<HalfFloat>::max();
    } else if (input == half{0.0}) {
        return half{1.0};
    }
    auto mult_out = input * log2e;
    auto p = GetUV(mult_out);
    half result = LinearExp2(p.first);

    uint16_t frac = (result.GetBits() & 0x3ff);
    uint16_t exp = ((result.GetBits() >> 10) & 0x1f);
    uint16_t sign = ((result.GetBits() >> 15) & 0x1);

    exp = (uint16_t)((int)exp + p.second);

    return {frac, exp, sign};
}

half Exponent::Calculate(float input) {
    return Calculate(half{input});
}

half Exponent::Calculate(double input) {
    return Calculate(half{input});
}

std::pair<half, int> Exponent::GetUV(const half& input) {
    int exp_num = ((input.GetBits() >> 10) & 0x1f) - 15;
    uint16_t frac_num = (input.GetBits() & 0x3ff) | 0x400;
    uint16_t sign_num = ((input.GetBits() >> 15) & 0x1);
    if (exp_num >= 0) {
        frac_num <<= exp_num;
    } else {
        frac_num >>= (-exp_num);
    }
    int u = (frac_num >> 10);
    uint16_t v = (frac_num & 0x3ff);
    if (sign_num == 0x1) {
        u = -u;
        if (v != 0) {
            u--;
            v = 0x400 - v;
        }
    }

    for (uint16_t e = 0xe, i = 0; i < 10; i++, e--) {
        v <<= 1;
        if (v & 0x400) {
            return {half((v & 0x3ff), e, 0x0), u};
        }
    }
    return {half{0.0}, u};
}

LinearFunctionParameters Exponent::GetLinearExp2Params(int n, bool print_params) {
    LinearFunctionParameters params;
    for (int i = 0; i < n; i++) {
        double x1 = (i + 0.0) / n;
        double x2 = (i + 1.0) / n;
        double k = (exp2(x2) - exp2(x1)) / (x2 - x1);
        double b1 = exp2(x1) - k * x1;

        double x_ = log2(k) - log2(log(2));
        double b2 = exp2(x_) - k * x_;

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

half Exponent::LinearExp2(const half& x) {
    for (auto& param : params_) {
        if (param["x1"] <= x && x < param["x2"]) {
            return x * param["k"] + param["b"];
        }
    }
    return half{0.0};
}

}  // namespace module

}  // namespace softmax
