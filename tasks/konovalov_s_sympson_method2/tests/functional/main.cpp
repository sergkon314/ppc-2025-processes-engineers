#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <numbers>
#include <string>
#include <tuple>
#include <vector>

#include "konovalov_s_sympson_method/common/include/common.hpp"
#include "konovalov_s_sympson_method/mpi/include/ops_mpi.hpp"
#include "konovalov_s_sympson_method/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_sympson_method {

class KonovalovSSympsonMethodFuncTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return "ftest_" + std::get<5>(test_param);
  }

 protected:
  void SetUp() override {
    input_data_ = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data > 0;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
};

namespace {

TEST_P(KonovalovSSympsonMethodFuncTests, SympsonMPTests) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 2> kTestParam = {std::make_tuple(0.1, 4.9, 2.4, 2.5, 10),
                                            std::make_tuple(8.3, 10.2, 3.2, 6.7, 100)};

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KonovalovSSympsonMethodMPI, InType>(kTestParam, PPC_SETTINGS_konovalov_s_sympson_method),
    ppc::util::AddFuncTask<KonovalovSSympsonMethodSEQ, InType>(kTestParam, PPC_SETTINGS_konovalov_s_sympson_method));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = KonovalovSSympsonMethodFuncTests::PrintFuncTestName<KonovalovSSympsonMethodFuncTests>;

INSTANTIATE_TEST_SUITE_P(SympsonMPTest, KonovalovSSympsonMethodFuncTests, kGtestValues, kPerfTestName);

class KonovalovSSympsonMethodValidationTests : public ::testing::Test {};

}  // namespace

}  // namespace konovalov_s_sympson_method
