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
    std::string test_name = std::get<0>(test_param);
    return test_name;
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());

    int _size = std::get<1>(params);
    input_data_ = _size;
    incorrect_output.resize(_size, 0.0);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    std::cout << __FILE__ << ":" << __LINE__ << ": output: ";
    for (auto i: output_data){
      std::cout  << i << " ";
    }
     std::cout << std::endl;
    return output_data != incorrect_output;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType incorrect_output;
};

namespace {

TEST_P(KonovalovSRunFuncTestsProcesses2, SeidelMethodF) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 3> kTestParam = {std::make_tuple("sys_1", 32), std::make_tuple("sys_2", 64),
                                            std::make_tuple("sys_3", 128)};

const auto kTestTasksList = std::tuple_cat(ppc::util::AddFuncTask<KonovalovSSeidelMethodMPI, InType>(
                                               kTestParam, PPC_SETTINGS_konovalov_s_seidel_iterative_method),
                                           ppc::util::AddFuncTask<KonovalovSSeidelMethodSEQ, InType>(
                                               kTestParam, PPC_SETTINGS_konovalov_s_seidel_iterative_method));

// const auto kTestTasksList =
//     std::tuple_cat(
//                    ppc::util::AddFuncTask<KonovalovSSeidelMethodSEQ, InType>(kTestParam,
//                    PPC_SETTINGS_konovalov_s_seidel_iterative_method));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KonovalovSRunFuncTestsProcesses2::PrintFuncTestName<KonovalovSRunFuncTestsProcesses2>;

INSTANTIATE_TEST_SUITE_P(LinearSysOfEqFromFileF, KonovalovSRunFuncTestsProcesses2, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace konovalov_s_seidel_iterative_method
