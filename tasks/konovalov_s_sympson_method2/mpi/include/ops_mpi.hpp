#pragma once

#include <functional>
#include <vector>

#include "task/include/task.hpp"
#include "konovalov_s_sympson_method2/common/include/common.hpp"

namespace konovalov_s_sympson_method2 {

class KonovalovSSympsonMethodMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit KonovalovSSympsonMethodMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

  double ComputePartialIntegral(int start_idx, int end_idx);

  std::vector<double> lower_bounds_;
  std::vector<double> upper_bounds_;
  int num_steps_{};
  std::function<double(const std::vector<double> &)> func_;
  double result_{};
  int start_idx_{};
  int end_idx_{};
};

}  // namespace konovalov_s_sympson_method2