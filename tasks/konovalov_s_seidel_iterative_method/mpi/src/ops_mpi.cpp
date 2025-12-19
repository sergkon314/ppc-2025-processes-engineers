#include "konovalov_s_seidel_iterative_method/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "konovalov_s_seidel_iterative_method/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_seidel_iterative_method {

KonovalovSSeidelMethodMPI::KonovalovSSeidelMethodMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::vector<double>(3, 0.0);
}

bool KonovalovSSeidelMethodMPI::ValidationImpl() {
  // size = std::get<0>(GetInput());
  // A.resize(size, 0.0);
  // A = std::get<1>(GetInput());
  // B.resize(size);
  // B = std::get<2>(GetInput());
  // iter = std::get<3>(GetInput());
  return true;
}

bool KonovalovSSeidelMethodMPI::PreProcessingImpl() {
  if (A.size() == 0 || B.size() == 0 || size == 0 || iter == 0) {
    return false;
  }
  for (int i = 0; i < size; i++) {
    if (A[i * size + i] == 0) {
      return false;
    }
  }
  return true;
}

std::vector<double> KonovalovSSeidelMethodMPI::IterationProcess(std::vector<double> &_A, std::vector<double> &_B,
                                                                double X0, int _iter, double _epsi) {
  double epsi = _epsi;
  int iter = _iter;
  std::vector<double> X(_B.size(), X0);

  while (iter != 0) {
    bool flag = true;

    std::vector<double> X_new(_B.size(), 0.0);
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
      break;
    }
  }

  return X;
}

bool KonovalovSSeidelMethodMPI::RunImpl() {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // MPI_Datatype custom_vec;
  if (rank == 0) {
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
  //   GetOutput() =
  return true;
}

bool KonovalovSSeidelMethodMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
