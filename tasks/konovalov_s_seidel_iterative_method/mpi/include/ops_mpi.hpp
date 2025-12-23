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
  static std::vector<double> IterationStep(int mtr_szie, int diff, int rank, std::vector<int> &_A, std::vector<int> &_B, std::vector<double> &X_gl);
  void InitMatrixA(int size, int fmax, std::vector<int> &A, int diff);
  void InitMatrixB(int size, int fmax, std::vector<int> &B);
  void DataDistr(int size, int diff, int mtr_size);
  void DataRecv(std::vector<int> &A_local, std::vector<int> &B_local, int matrix_size, int diff);
  std::vector<int> Coloring(int size, std::vector<int> &_A);
};

}  // namespace konovalov_s_seidel_iterative_method
