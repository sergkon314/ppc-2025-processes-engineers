#pragma once

#include "konovalov_s_simpson_method/common/include/common.hpp"
#include "task/include/task.hpp"

namespace konovalov_s_simpson_method {

class KonovalovSSimpsonMethodMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit KonovalovSSimpsonMethodMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace konovalov_s_simpson_method
