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
  int size = 0;
  int max_iter = 0;
  int iter = 0;

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  static std::vector<double> IterationProcess(std::vector<double> &_A, std::vector<double> &_B, int _iter,
                                     double _epsi);
  void InitMatrixA(int size, int fmax, std::vector<double> &A, int diff);
  void InitMatrixB(int size, int fmax, std::vector<double> &B);
  std::vector<int> Coloring(int size, std::vector<double> _A);
};

}  // namespace konovalov_s_seidel_iterative_method
