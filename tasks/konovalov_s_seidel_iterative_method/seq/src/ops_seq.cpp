#include "konovalov_s_seidel_iterative_method/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_seidel_iterative_method {

KonovalovSSeidelMethodSEQ::KonovalovSSeidelMethodSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::vector<double>(size, 0.0);
}

bool KonovalovSSeidelMethodSEQ::ValidationImpl() {
  if(GetInput() < 3) return false;
  return true;
}

bool KonovalovSSeidelMethodSEQ::PreProcessingImpl() {
  size = GetInput();
  
  return true;
}

void KonovalovSSeidelMethodSEQ::InitMatrixA(long unsigned int size, int fmax, std::vector<double> &A) {
  A.resize(size*size, 0.0);
  double x = 1 + rand() % fmax;
  for (long unsigned int i = 0; i < size; i++) {
    if (i != 0) {
      A[i * size + i - 1] = x;
    }
    if (i != size) {
      A[i * size + i + 1] = x;
    }
    A[i * size + i] = x * 2 + 1;
  }
}

void KonovalovSSeidelMethodSEQ::InitMatrixB(long unsigned int size, int fmax, std::vector<double> &B) {
  for (long unsigned int i = 0; i < size; i++) {
    B[i] = 1 + rand() % fmax;
  }
}

std::vector<double> KonovalovSSeidelMethodSEQ::IterationProcess(std::vector<double> &_A, std::vector<double> &_B, double X0, int _iter,
                                     double _epsi) {
  double epsi = _epsi;
  int iter = _iter;
  std::vector<double> X(_B.size(), X0);

  std::vector<double> X_new(_B.size(), 0.0);

  while (iter != 0) {
    bool flag = true;
    for (long unsigned int i = 0; i < _B.size(); i++) {
      X_new[i] = _B[i] / _A[i * _B.size() + i];
      for (long unsigned int j = 0; j < _B.size(); j++) {
        if (j == i) {
          continue;
        }
        X_new[i] -= (_A[i * _B.size() + j] / _A[i * _B.size() + i]) * X[j];
      }

      flag = flag && (std::fabs(X_new[i] - X[i]) < epsi);

      X[i] = round(X_new[i] * 1000) / 1000;
    }
    iter--;

    if (flag) break;
    
  }
  return X;
}

bool KonovalovSSeidelMethodSEQ::RunImpl() {
  std::vector<double> A(size*size, 0.0);
  std::vector<double> B(size, 0.0);
  InitMatrixA(GetInput(), 100, A);
  InitMatrixB(GetInput(), 100, B);
  GetOutput() = IterationProcess(A, B, 0, iter, 0.001);

  return true;
  
}

bool KonovalovSSeidelMethodSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
