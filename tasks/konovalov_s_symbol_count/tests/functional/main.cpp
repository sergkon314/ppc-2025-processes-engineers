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

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "konovalov_s_symbol_count/mpi/include/ops_mpi.hpp"
#include "konovalov_s_symbol_count/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_symbol_count {

class KonovalovSSymbolCountFuncTest : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    int fndot = std::get<1>(test_param).find(".");
    return std::get<1>(test_param).substr(0, fndot);
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    std::string data4test = ppc::util::GetAbsoluteTaskPath(PPC_ID_konovalov_s_symbol_count, std::get<1>(params));

    std::ifstream in(data4test);
    while (std::getline(in, input_data_)){}

    correct_output = std::get<0>(params);
    

    in.close();
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == correct_output;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType correct_output;
};

namespace {

TEST_P(KonovalovSSymbolCountFuncTest, SymbolsInLineCountFT) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 3> kTestParam = {std::make_tuple(15, "text_line_15.txt"), std::make_tuple(337, "text_line_337.txt"),std::make_tuple(681, "text_line_681.txt")};

const auto kTestTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<KonovalovSSymbolCountMPI, InType>(kTestParam, PPC_SETTINGS_konovalov_s_symbol_count),
                   ppc::util::AddFuncTask<KonovalovSSymbolCountSEQ, InType>(kTestParam, PPC_SETTINGS_konovalov_s_symbol_count));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KonovalovSSymbolCountFuncTest::PrintFuncTestName<KonovalovSSymbolCountFuncTest>;

INSTANTIATE_TEST_SUITE_P(FTestData, KonovalovSSymbolCountFuncTest, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace konovalov_s_symbol_count
