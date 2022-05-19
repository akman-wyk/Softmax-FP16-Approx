//
// Created by Akman on 2022/3/22.
//

#pragma once
#include "type.h"

namespace softmax {

namespace module {

class Logarithm {
public:
    static const half ln2;

    static LinearFunctionParameters GetLinearLog2Params(int n, bool print_params = false);

public:
    explicit Logarithm(int n) : params_(GetLinearLog2Params(n)) {}

    half LinearLog2(const half& x);

    half Calculate(const half& input);

private:
    LinearFunctionParameters params_;
};

}  // namespace module

}  // namespace softmax
