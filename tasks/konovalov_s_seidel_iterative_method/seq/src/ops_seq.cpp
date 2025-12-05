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
  /*
  if(std::get<3>(GetInput()) < 0) return false;
  if(std::get<2>(GetInput()).size() != std::get<0>(GetInput()) ||
    std::get<1>(GetInput()).size() != std::get<0>(GetInput())) 
    return false;*/
  return true;
}

bool KonovalovSSeidelMethodSEQ::PreProcessingImpl() {
  size = std::get<0>(GetInput());
  A.resize(size, std::vector<double>(size, 0.0));
  A = std::get<1>(GetInput());
  B.resize(size);
  B = std::get<2>(GetInput());
  iter = std::get<3>(GetInput());
  return true;
}

bool KonovalovSSeidelMethodSEQ::RunImpl() {
  std::vector<double> X(size, 0.0),  X_new(size, 0.0);

  while (iter != 0){

    for(int i = 0; i < size; i++){
      X_new[i] = B[i]/A[i][i];

      for(int j = 0; j < size; j++){
        if(j == i) continue;
        X_new[i] -= (A[i][j]/A[i][i]) * X[i];
        X[i] = round(X_new[i]*1000)/1000;
      }
    }
    iter--;

    
  }


  GetOutput() = X;
  return true;
}

bool KonovalovSSeidelMethodSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
