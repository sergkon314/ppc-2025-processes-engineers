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

#include "konovalov_s_simpson_method/common/include/common.hpp"
#include "konovalov_s_simpson_method/mpi/include/ops_mpi.hpp"
#include "konovalov_s_simpson_method/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_simpson_method {

class KonovalovSRunFuncTestsProcesses3 : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return "ftest_for_" + std::to_string(std::get<2>(test_param));
  }

 protected:
  void SetUp() override {
    input_data_ = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
  }

  bool CheckTestOutputData(OutType &output_data) final {
    std::cout << output_data << std::endl;
    return (output_data != 0);
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
};

namespace {

TEST_P(KonovalovSRunFuncTestsProcesses3, SimpsonFTests) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 4> kTestParam = {std::make_tuple(0.0, 4.0, 2), std::make_tuple(4.6, 45.1, 4), std::make_tuple(90.3, 145.8, 8), std::make_tuple(322.1, 362.4, 10)};

const auto kTestTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<KonovalovSSimpsonMethodMPI, InType>(kTestParam, PPC_SETTINGS_konovalov_s_simpson_method),
                   ppc::util::AddFuncTask<KonovalovSSimpsonMethodSEQ, InType>(kTestParam, PPC_SETTINGS_konovalov_s_simpson_method));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KonovalovSRunFuncTestsProcesses3::PrintFuncTestName<KonovalovSRunFuncTestsProcesses3>;

INSTANTIATE_TEST_SUITE_P(RunSimpsonFTest, KonovalovSRunFuncTestsProcesses3, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace konovalov_s_simpson_method
