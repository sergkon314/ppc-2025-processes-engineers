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

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  static std::vector<double> IterationProcess(int mtr_szie, int diff, int rank, std::vector<int> &_A, std::vector<int> &_B);
  void InitMatrixA(int size, int fmax, std::vector<int> &A, int diff);
  void InitMatrixB(int size, int fmax, std::vector<int> &B);
  std::vector<int> Coloring(int size, std::vector<int> &_A);
};

}  // namespace konovalov_s_seidel_iterative_method
