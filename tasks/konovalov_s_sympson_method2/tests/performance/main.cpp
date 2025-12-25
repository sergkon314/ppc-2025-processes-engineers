#include <gtest/gtest.h>

#include <cmath>
#include <cstddef>
#include <vector>

#include "konovalov_s_sympson_method2/common/include/common.hpp"
#include "konovalov_s_sympson_method2/mpi/include/ops_mpi.hpp"
#include "konovalov_s_sympson_method2/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace konovalov_s_sympson_method2 {

namespace {

double PerfFunc(const std::vector<double> &point) {
  double sum = 0.0;
  for (std::size_t idx = 0; idx < point.size(); ++idx) {
    sum += (std::sin(point[idx]) * std::cos(point[idx])) + (point[idx] * point[idx]);
  }
  return sum;
}

}  // namespace

class KonovalovSSympsonMethodPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_{};

  void SetUp() override {
    input_data_.lower_bounds = {0.0, 0.0, 0.0};
    input_data_.upper_bounds = {1.0, 1.0, 1.0};
    input_data_.num_steps = 100;
    input_data_.func = PerfFunc;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data > 0.0;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KonovalovSSympsonMethodPerfTests, SympsonMPTests) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KonovalovSSympsonMethodMPI, KonovalovSSympsonMethodSEQ>(
        PPC_SETTINGS_konovalov_s_sympson_method2);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KonovalovSSympsonMethodPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(SympsonMPTest, KonovalovSSympsonMethodPerfTests, kGtestValues, kPerfTestName);

}  // namespace konovalov_s_sympson_method2