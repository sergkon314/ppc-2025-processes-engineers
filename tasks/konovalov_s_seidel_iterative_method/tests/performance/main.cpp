#include <gtest/gtest.h>

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "konovalov_s_seidel_iterative_method/mpi/include/ops_mpi.hpp"
#include "konovalov_s_seidel_iterative_method/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace konovalov_s_seidel_iterative_method {

class  KonovalovSRunPerfTestsProcesses2 : public ppc::util::BaseRunPerfTests<InType, OutType> {
  void SetUp() override {
    input_data_ = 2048;
    incorrect_output.resize(input_data_, 0.0);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data != incorrect_output;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType incorrect_output;
};

TEST_P( KonovalovSRunPerfTestsProcesses2, SeidelPTests) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, KonovalovSSeidelMethodMPI, KonovalovSSeidelMethodSEQ>(
    PPC_SETTINGS_konovalov_s_seidel_iterative_method);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName =  KonovalovSRunPerfTestsProcesses2::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(SeidelPTestRun,  KonovalovSRunPerfTestsProcesses2, kGtestValues, kPerfTestName);

}  // namespace konovalov_s_seidel_iterative_method
