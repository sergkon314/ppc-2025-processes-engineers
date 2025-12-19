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
  std::vector<double> A;
  std::vector<double> B;
  int max_iter = 0;
  int iter = 0;

  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
  static std::vector<double> IterationProcess(std::vector<double> &_A, std::vector<double> &_B, double X0, int _iter,
                                     double _epsi);
  void InitMatrixA(long unsigned int size, int fmax, std::vector<double> &A);
  void InitMatrixB(long unsigned int size, int fmax, std::vector<double> &B);
  
};

}  // namespace konovalov_s_seidel_iterative_method
