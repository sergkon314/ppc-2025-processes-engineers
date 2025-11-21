#include <gtest/gtest.h>

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "konovalov_s_symbol_count/mpi/include/ops_mpi.hpp"
#include "konovalov_s_symbol_count/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace konovalov_s_symbol_count {

class ExampleRunPerfTestProcesses : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};

  void SetUp() override {
    input_data_ = kCount_;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return input_data_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(ExampleRunPerfTestProcesses, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KonovalovSSymbolCountMPI, KonovalovSSymbolCountSEQ>(PPC_SETTINGS_konovalov_s_symbol_count);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = ExampleRunPerfTestProcesses::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, ExampleRunPerfTestProcesses, kGtestValues, kPerfTestName);

}  // namespace konovalov_s_symbol_count
