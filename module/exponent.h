//
// Created by Akman on 2022/3/21.
//

#pragma once

#include "type.h"

namespace softmax {

namespace module {

class Exponent {
public:
    static const half min_input;
    static const half max_input;
    static const half log2e;

    static std::pair<half, int> GetUV(const half& input);

    static LinearFunctionParameters GetLinearExp2Params(int n, bool print_params = false);

public:
    explicit Exponent(int n) : params_(GetLinearExp2Params(n)) {}

    half LinearExp2(const half& x);

    half Calculate(const half& input);

    half Calculate(float input);

    half Calculate(double input);

private:
    LinearFunctionParameters params_;
};

}  // namespace module

}  // namespace softmax
