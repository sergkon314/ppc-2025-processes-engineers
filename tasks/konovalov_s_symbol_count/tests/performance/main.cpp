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
    // TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    // std::string data4test = std::get<1>(params);
    
    // std::ifstream in;
    // in.open(data4test);
    // std::getline(in, input_data_);

    // correct_output = std::get<0>(params);
    

    // in.close();

    correct_output = 1500;

    for(std::uint32_t i = 0; i < 1500; i++)
      input_data_ += "P/1.";

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
