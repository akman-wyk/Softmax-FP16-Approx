#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "half_float/half_float.h"
#include "module/exponent.h"
#include "module/logarithm.h"
#include "module/util.h"
#include "softmax.h"

const int DATA_SET_NUM = 10000;
const int DATA_NUM_PER_SET = 10;
const double DATA_NUM_PER_SET_LF = 10.0;

const std::string INPUT_FILE = "input_data";

const bool ACCURACY_TEST = false;
const std::string ACCURACY_TEST_LOSS_FILE = "../loss";

const bool PARAM_CONFIG_TEST = false;
const std::string EXP_LOSS_FILE = "../exp_loss";
const std::string LOG_LOSS_FILE = "../log_loss";

std::vector<double> std_softmax(std::vector<half> inputs) {
    half max = std::numeric_limits<HalfFloat>::min();
    for (auto& input : inputs) {
        if (max < input) {
            max = input;
        }
    }

    for (auto& input : inputs) {
        input -= max;
    }

    std::vector<half> results;
    half sum_exp{0.0};
    for (auto& input : inputs) {
        half exp_input = half(exp((double)input));
        results.emplace_back(exp_input);
        sum_exp += exp_input;
    }
    for (auto& result : results) {
        result /= sum_exp;
    }
    std::vector<double> d_results;
    for (auto& r : results) {
        d_results.emplace_back((double)r);
    }
    return std::move(d_results);
}

std::vector<std::vector<half>> GetInputSets(const std::string& input_file, int data_set_num,
                                            int data_num_per_set) {
    std::ifstream is;
    is.open(input_file, std::ios_base::in);
    std::vector<std::vector<half>> input_sets;
    while (data_set_num--) {
        std::vector<half> inputs;
        for (int i = 0; i < data_num_per_set; i++) {
            double input;
            is >> input;
            inputs.emplace_back(half(input));
        }
        input_sets.emplace_back(std::move(inputs));
    }
    is.close();
    return std::move(input_sets);
}

double Test(int exp_linear_num, int log_linear_num,
            const std::vector<std::vector<half>>& input_sets, bool write_loss = false,
            const std::string& loss_file = ACCURACY_TEST_LOSS_FILE,
            double data_num_per_set = DATA_NUM_PER_SET_LF) {
    softmax::Softmax softmax(exp_linear_num, log_linear_num);
    int n = 10;
    double total_loss = 0.0;
    std::ofstream os;
    if (write_loss) {
        os.open(loss_file, std::ios_base::out);
    }
    for (auto& inputs : input_sets) {
        auto r1 = softmax.CalculateDouble(inputs);
        auto r2 = std_softmax(inputs);
        double loss = 0.0;
        for (int i = 0; i < n; i++) {
            loss += std::abs(r1[i] - r2[i]);
        }
        if (write_loss) {
            os << loss / data_num_per_set << std::endl;
        }
        total_loss += loss;
    }
    if (write_loss) {
        os.close();
    }
    return total_loss;
}

void ParamsConfigTest(const std::vector<std::vector<half>>& input_sets,
                      const std::string& exp_loss_file, const std::string& log_loss_file) {
    std::ofstream os1;
    os1.open(exp_loss_file);
    std::cout << "exp config test" << std::endl;
    for (int i = 1; i <= 100; i++) {
        double tot = Test(i, 10, input_sets);
        std::cout << tot << std::endl;
        os1 << tot << std::endl;
    }
    os1.close();

    std::ofstream os2;
    os2.open(log_loss_file);
    std::cout << "log config test" << std::endl;
    for (int i = 1; i <= 100; i++) {
        double tot = Test(10, i, input_sets);
        std::cout << tot << std::endl;
        os2 << tot << std::endl;
    }
    os2.close();
}

int main() {
    auto input_sets = GetInputSets(INPUT_FILE, DATA_SET_NUM, DATA_NUM_PER_SET);
    if (ACCURACY_TEST) {
        Test(10, 50, input_sets, true, ACCURACY_TEST_LOSS_FILE, DATA_NUM_PER_SET_LF);
    }
    if (PARAM_CONFIG_TEST) {
        ParamsConfigTest(input_sets, EXP_LOSS_FILE, LOG_LOSS_FILE);
    }
    return 0;
}
