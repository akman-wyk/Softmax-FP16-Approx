//
// Created by Akman on 2022/3/22.
//

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "half_float.h"

namespace softmax {

namespace module {

using LinearFunctionParameter = std::unordered_map<std::string, half>;
using LinearFunctionParameters = std::vector<LinearFunctionParameter>;

}  // namespace module

}  // namespace softmax
