//
// Created by Akman on 2022/3/22.
//

#pragma once
#include <iostream>
#include <vector>

#include "half_float.h"
#include "module/exponent.h"
#include "module/logarithm.h"

namespace softmax {

class Softmax {
public:
    Softmax(int exp_linear_num, int log_linear_num)
        : exponent_(exp_linear_num), logarithm_(log_linear_num) {}

    std::vector<double> CalculateDouble(std::vector<half> inputs) {
        std::vector<half> result = CalculateHalf(std::move(inputs));

        std::vector<double> d_results;
        for (auto& r : result) {
            d_results.emplace_back((double)r);
        }
        return std::move(d_results);
    }

    std::vector<half> CalculateHalf(std::vector<half> inputs) {
        half max = std::numeric_limits<HalfFloat>::min();
        for (auto& input : inputs) {
            if (max < input) {
                max = input;
            }
        }

        for (auto& input : inputs) {
            input -= max;
        }

        half sum = half(0.0);
        for (auto& input : inputs) {
            half r = exponent_.Calculate(input);
            sum = sum + r;
        }
        half ln_sum = logarithm_.Calculate(sum);

        std::vector<half> result;
        result.reserve(inputs.size());
        for (auto& input : inputs) {
            result.emplace_back(exponent_.Calculate(input - ln_sum));
        }
        return std::move(result);
    }

private:
    module::Exponent exponent_;
    module::Logarithm logarithm_;
};

}  // namespace softmax
