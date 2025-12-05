#pragma once

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "task/include/task.hpp"

namespace konovalov_s_seidel_iterative_method {

class KonovalovSSeidelMethodSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit KonovalovSSeidelMethodSEQ(const InType &in);

 private:
  int size;
  std::vector<std::vector<double>> A;
  std::vector<double> B;
  int max_iter;
  int iter;

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace konovalov_s_seidel_iterative_method
