#pragma once

#include <functional>
#include <vector>

#include "task/include/task.hpp"
#include "konovalov_s_sympson_method2/common/include/common.hpp"

namespace konovalov_s_sympson_method2 {

class KonovalovSSympsonMethodSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit KonovalovSSympsonMethodSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  double GetFunc(double x, double y);
  double SimpsonMethod(double a, double b, double c, double d, int n);
};

}  // namespace konovalov_s_sympson_method2