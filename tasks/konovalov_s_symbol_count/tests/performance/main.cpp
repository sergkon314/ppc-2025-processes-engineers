#include <gtest/gtest.h>

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "konovalov_s_symbol_count/mpi/include/ops_mpi.hpp"
#include "konovalov_s_symbol_count/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace konovalov_s_symbol_count {

class KonovalovSSymbolCountPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_;
  OutType correct_output = 0;

  void SetUp() override {
    std::string str;
    for(int i = 0; i < 100000000; i++){
      str += "0";
    }
    str += "t";
    input_data_ = str;
    correct_output = 1;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == correct_output;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KonovalovSSymbolCountPerfTest, SymbolsInLineCounPT) {
  ExecuteTest(GetParam());
}



const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KonovalovSSymbolCountMPI, KonovalovSSymbolCountSEQ>(PPC_SETTINGS_konovalov_s_symbol_count);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KonovalovSSymbolCountPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(PTestData, KonovalovSSymbolCountPerfTest, kGtestValues, kPerfTestName);

}  // namespace konovalov_s_symbol_count
