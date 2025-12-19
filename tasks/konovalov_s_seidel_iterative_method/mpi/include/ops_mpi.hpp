#pragma once

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "task/include/task.hpp"

namespace konovalov_s_seidel_iterative_method {

class KonovalovSSeidelMethodMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit KonovalovSSeidelMethodMPI(const InType &in);

 private:
  int size;
  std::vector<double> A;
  std::vector<double> B;
  int max_iter;
  int iter;

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  std::vector<double> IterationProcess(std::vector<double> &_A, std::vector<double> &_B, double X0, int _iter,
                                     double _epsi);
};

}  // namespace konovalov_s_seidel_iterative_method
