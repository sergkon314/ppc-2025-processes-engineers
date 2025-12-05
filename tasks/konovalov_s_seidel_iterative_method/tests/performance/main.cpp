#include <gtest/gtest.h>

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "konovalov_s_seidel_iterative_method/mpi/include/ops_mpi.hpp"
#include "konovalov_s_seidel_iterative_method/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace konovalov_s_seidel_iterative_method {

class ExampleRunPerfTestProcesses2 : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};
  std::vector<double> s;
  void SetUp() override {
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == s;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(ExampleRunPerfTestProcesses2, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KonovalovSSeidelMethodMPI, KonovalovSSeidelMethodSEQ>(PPC_SETTINGS_konovalov_s_seidel_iterative_method);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = ExampleRunPerfTestProcesses2::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, ExampleRunPerfTestProcesses2, kGtestValues, kPerfTestName);

}  // namespace konovalov_s_seidel_iterative_method
