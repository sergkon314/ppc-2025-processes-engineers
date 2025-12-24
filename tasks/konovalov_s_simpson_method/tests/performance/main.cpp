#include <gtest/gtest.h>

#include "konovalov_s_simpson_method/common/include/common.hpp"
#include "konovalov_s_simpson_method/mpi/include/ops_mpi.hpp"
#include "konovalov_s_simpson_method/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace konovalov_s_simpson_method {

class KonovalovSRunPerfTestProcesses3 : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_{};

  void SetUp() override {
    input_data_ = std::make_tuple(0.0, 0.0, 0);

  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == 0;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KonovalovSRunPerfTestProcesses3, SimpsonPTests) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KonovalovSSimpsonMethodMPI, KonovalovSSimpsonMethodSEQ>(PPC_SETTINGS_konovalov_s_simpson_method);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KonovalovSRunPerfTestProcesses3::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunSimpsonPTest, KonovalovSRunPerfTestProcesses3, kGtestValues, kPerfTestName);

}  // namespace konovalov_s_simpson_method
