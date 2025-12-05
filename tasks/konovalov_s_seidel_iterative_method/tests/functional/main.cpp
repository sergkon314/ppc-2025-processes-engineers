#include <gtest/gtest.h>
#include <stb/stb_image.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "konovalov_s_seidel_iterative_method/mpi/include/ops_mpi.hpp"
#include "konovalov_s_seidel_iterative_method/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_seidel_iterative_method {

class KonovalovSRunFuncTestsProcesses2 : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    int fndot = test_param.find(".");
    return test_param.substr(0, fndot);
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    std::string data4test = ppc::util::GetAbsoluteTaskPath(PPC_SETTINGS_konovalov_s_seidel_iterative_method, params);
    
    std::string raw_data;
    std::ifstream in(data4test);
    std::vector<std::string> segmlist;

    while(std::getline(in, raw_data, ' ')){
      segmlist.push_back(raw_data);
    }

    int _size = (int)raw_data[0];
    int _iter = (int)raw_data[1];
    std::vector<std::vector<double>> _A(_size, std::vector<double>(_size, 0.00));
    std::vector<double> _B(_size, 0.00);

    size_t count = 2;
    
    for(int i = 0; i < _size; i++){
      for(int j = 0; j < _size; j++){
        _A[i][j] = (double)raw_data[count];
        count++;
      } 
    }

    for(int i = 0; i < _size; i++){
      _B[i] = (double)raw_data[count];
      count++;
    }
    
    input_data_ = std::make_tuple(_size, _A, _B, _iter);

    correct_output.resize(_size, 0.000);

    for(int i = 0; i < _size; i++){
      correct_output[i] = (double)raw_data[count];
      count++;
    }
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return correct_output == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType correct_output;
};

namespace {

TEST_P(KonovalovSRunFuncTestsProcesses2, SeidelMethodF) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 1> kTestParam = {"sys_1.txt"};

const auto kTestTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<KonovalovSSeidelMethodMPI, InType>(kTestParam, PPC_SETTINGS_konovalov_s_seidel_iterative_method),
                   ppc::util::AddFuncTask<KonovalovSSeidelMethodSEQ, InType>(kTestParam, PPC_SETTINGS_konovalov_s_seidel_iterative_method));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KonovalovSRunFuncTestsProcesses2::PrintFuncTestName<KonovalovSRunFuncTestsProcesses2>;

INSTANTIATE_TEST_SUITE_P(LinearSysOfEqFromFileF, KonovalovSRunFuncTestsProcesses2, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace konovalov_s_seidel_iterative_method
