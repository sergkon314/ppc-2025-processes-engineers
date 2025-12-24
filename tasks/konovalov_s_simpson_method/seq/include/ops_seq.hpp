#pragma once

#include "konovalov_s_simpson_method/common/include/common.hpp"
#include "task/include/task.hpp"

namespace konovalov_s_simpson_method {

class KonovalovSSimpsonMethodSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit KonovalovSSimpsonMethodSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  double SimpsonM(double lower_lim, double upper_lim, int n);
  double SquareFuncVal(double x, double y);
};

}  // namespace konovalov_s_simpson_method
