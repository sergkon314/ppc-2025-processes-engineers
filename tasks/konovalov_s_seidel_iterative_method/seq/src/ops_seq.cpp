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
  if(GetInput() < 16) return false;
  return true;
}

bool KonovalovSSeidelMethodSEQ::PreProcessingImpl() {
  size = GetInput();
  
  return true;
}

void KonovalovSSeidelMethodSEQ::InitMatrixA(int size, int fmax, std::vector<double> &A, int _diff) {
  int diff = _diff;
  int x = rand() % fmax;

  for (int i = 0; i < size; i++) {
    int sum = 0;
    int j = 1;
    do {
      int diff_addr_pos = i * size + i + j;
      int diff_addr_neg = i * size + i - j;

      if (diff_addr_neg > 0 && diff_addr_neg >= i * size) {
        A[diff_addr_neg] = x * (diff - j);
        sum += A[diff_addr_neg];
      }

      if (diff_addr_pos < size*size && i + j < size) {
        A[diff_addr_pos] = x * (diff - j);
        sum += A[diff_addr_pos];
      }
      j++;
    } while (j < diff);
    A[i * size + i] = abs(sum + 1);
  }
  // std::cout << __FILE__ << ":" << __LINE__ << ": initial x: " << x << " " << std::endl;
}

void KonovalovSSeidelMethodSEQ::InitMatrixB(int size, int fmax, std::vector<double> &B) {
  for (int i = 0; i < size; i++) {
    B[i] = rand() % fmax;
  }
  
}

std::vector<double> KonovalovSSeidelMethodSEQ::IterationProcess(std::vector<double> &_A, std::vector<double> &_B, int _iter,
                                     double _epsi) {
  double epsi = _epsi;
  int iter = _iter;
  std::vector<double> X(_B.size(), 0.0);

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

    if (flag) {
      // std::cout << __FILE__ << ":" << __LINE__ << ": IP complete with iters left: " << iter << std::endl;
      
      break;
    }
  }
  return X;
}

bool KonovalovSSeidelMethodSEQ::RunImpl() {
  srand(time(NULL));

  std::vector<double> A(size*size, 0.0);
  std::vector<double> B(size, 0.0);
  InitMatrixA(size, 10, A, size/4);
  InitMatrixB(size, 10, B);
  GetOutput() = IterationProcess(A, B, 10, 0.001);

  return true;
  
}

bool KonovalovSSeidelMethodSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
