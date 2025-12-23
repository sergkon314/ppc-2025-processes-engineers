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
  GetOutput() = std::vector<double>(GetInput(), 0.0);
}

bool KonovalovSSeidelMethodMPI::ValidationImpl() {
  // size = std::get<0>(GetInput());
  // A.resize(size, 0.0);
  // A = std::get<1>(GetInput());
  // B.resize(size);
  // B = std::get<2>(GetInput());
  // iter = std::get<3>(GetInput());
  return GetInput() > 3;
}

bool KonovalovSSeidelMethodMPI::PreProcessingImpl() {
  return true;
}

void KonovalovSSeidelMethodMPI::InitMatrixA(int size, int fmax, std::vector<int> &A, int _diff) {
  int diff = _diff;
  int x = size + rand() % fmax;

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

      if (diff_addr_pos < size * size && i + j < size) {
        A[diff_addr_pos] = x * (diff - j);
        sum += A[diff_addr_pos];
      }
      j++;
    } while (j < diff);
    A[i * size + i] = abs(sum + 1);
  }
  std::cout << __FILE__ << ":" << __LINE__ << ": initial x: " << x << " " << std::endl;
}

void KonovalovSSeidelMethodMPI::InitMatrixB(int size, int fmax, std::vector<int> &B) {
  for (int i = 0; i < size; i++) {
    B[i] = pow(size + rand() % fmax, 2);
  }
}

std::vector<int> KonovalovSSeidelMethodMPI::Coloring(int size, std::vector<int> &_A) {
  std::vector<int> A = _A;       // coefficient matrix.int
  std::vector<int> color(size);  // colors of rows.
  for (int i = 0; i < size; ++i) {
    color[i] = -1;
  }
  for (int i = 0; i < size; ++i) {
    int m = 0;
    for (int j = 0; j < i; j++) {
      if (A[i * size + j] != 0 && color[j] == m) {
        ++m;
      }
    }
    color[i] = m;
  }
  std::cout << "colors ";
  for (auto i : color) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return color;
}

std::vector<double> KonovalovSSeidelMethodMPI::IterationStep(int mtr_size, int diff, int rank, std::vector<int> &_A,
                                                             std::vector<int> &_B, std::vector<double> &X_gl) {
  std::vector<double> X = X_gl;
  std::vector<double> X_ret(diff, 0.0);
  std::vector<double> X_new(mtr_size, 0.0);

  for (int i = 0; i < diff; i++) {
    int diag = (rank - 1) * (diff) + i;

    // std::cout << rank << " " << diag << std::endl;
    X_new[diag] = (double)_B[i] / (double)_A[diag + i * mtr_size];
    for (int j = 0; j < mtr_size; j++) {
      // std::cout << rank << " for " << i << std::endl;

      if (i * mtr_size + j == diag) {
        // std::cout << rank << " if " << j << std::endl;

        continue;
      }

      X_new[diag] -= ((double)_A[i * mtr_size + j] / (double)_A[diag + i * mtr_size]) * X[j];
    }

    X[diag] = round(X_new[diag] * 1000) / 1000;
    X_ret[i] = X[diag];
    // MPI_Barrier(MPI_COMM_WORLD);
    // std::cout << __FILE__ << ":" << __LINE__ << ": r: " << rank << " daig:" << diag << " B[i]: " << _B[i]
    //           << " A: " << _A[diag + i * mtr_size];
    // std::cout << " old: " << X[diag] << " stored: " << X_new[diag] << std::endl;
  }
  // iter--;

  // if (flag) {
  //   break;
  // }
  // }
  return X_ret;
}

void KonovalovSSeidelMethodMPI::DataDistr(int matrix_size, int size, int diff) {
  std::vector<int> A;
  std::vector<int> B;

  A.resize(matrix_size * matrix_size, 0);
  B.resize(matrix_size, 0);

  InitMatrixA(matrix_size, 10, A, matrix_size / (size - 1));
  InitMatrixB(matrix_size, 10, B);

  int r = matrix_size * diff;
  for (int i = 1; i < size; i++) {
    int p = (i - 1) * r;
    MPI_Send(&A[p], r, MPI_INT, i, 0, MPI_COMM_WORLD);
    MPI_Send(&B[(i - 1) * diff], diff, MPI_INT, i, 0, MPI_COMM_WORLD);
  }
}

void KonovalovSSeidelMethodMPI::DataRecv(std::vector<int> &A_local, std::vector<int> &B_local, int matrix_size,
                                         int diff) {
  MPI_Status s;
  A_local.resize(matrix_size * diff);
  B_local.resize(diff);

  MPI_Recv(&A_local[0], matrix_size * diff, MPI_INT, 0, 0, MPI_COMM_WORLD, &s);
  MPI_Recv(&B_local[0], diff, MPI_INT, 0, 0, MPI_COMM_WORLD, &s);
  // std::cout << "size: " << A_local.size() << std::endl;
  // std::cout << "mtr size:  " << matrix_size << std::endl;
  // std::cout << " local A: " << std::endl;
  // int c = 0;
  // for (int i = 0; i < matrix_size * diff; i++) {
  //   std::cout << A_local[i] << " ";
  //   if (i % matrix_size == matrix_size - 1) {
  //     std::cout << B_local[c] << std::endl;
  //     c++;
  //   }
  // }
  // std::cout << std::endl;
}

bool KonovalovSSeidelMethodMPI::RunImpl() {
  srand(time(NULL));

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int matrix_size = GetInput();
  int diff = matrix_size / (size - 1);
  // std::cout << rank << " diff " << diff << std::endl;

  std::vector<double> gl_x_vec(size * diff, 0.0);
  std::vector<double> local_x_vec(diff, 0.0);
  std::vector<int> colors(matrix_size);
  std::vector<int> A_local;
  std::vector<int> B_local;
  double epsi = 0.001;
  int iter = 10;
  bool stop_calc = true;

  gl_x_vec.resize(matrix_size, 0.0);
  colors.resize(matrix_size, 0);

  if (rank == 0) {
    DataDistr(matrix_size, size, diff);
  } else {
    DataRecv(A_local, B_local, matrix_size, diff);
  }

  while (iter != 0) {
    bool cover_tracker = true;
    if (rank != 0) {
      local_x_vec = IterationStep(matrix_size, diff, rank, A_local, B_local, gl_x_vec);

      for (int i = 0; i < diff; i++) {
        cover_tracker = cover_tracker && (std::fabs(local_x_vec[i] - gl_x_vec[(rank)*diff + i]) < epsi);
      }
      // std::cout << "p" << rank << " local X: " << std::endl;
      // for (int i = 0; i < diff; i++) {
      //   std::cout << local_x_vec[i] << " ";
      // }
      // std::cout << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
      std::vector<int> counts(size, diff);

      // std::cout << "p" << rank << " counts: " << counts.data() << std::endl;

      std::vector<int> displacements(size);

      for (int i = 0; i < size; i++) {
        displacements[i] = diff * i;
      }

      // std::cout << "p" << rank << " displ: ";  //<< displacements.data() << std::endl;
      // for (int i = 0; i < size; i++) {
      //   std::cout << displacements[i] << " ";
      // }
      // std::cout << std::endl;

      MPI_Gatherv(&local_x_vec[0], diff, MPI_DOUBLE, &gl_x_vec[0], counts.data(), displacements.data(), MPI_DOUBLE,
      0,
                  MPI_COMM_WORLD);
      // std::cout << std::endl;
      // std::cout << "p" << rank << " gl X: " << std::endl;
      // for (int i = 0; i < size * diff; i++) {
      //   std::cout << gl_x_vec[i] << " ";
      // }
      // std::cout << std::endl;
    } else {
      MPI_Gatherv(&local_x_vec[0], diff, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(&gl_x_vec[0], size * diff, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Allreduce(&cover_tracker, &stop_calc, 1, MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);
    if (stop_calc) {
      std::cout << "iter : " << iter << std::endl;
      break;
    }
    iter--;
  }
  for (int i = diff; i < size * diff; i++) {
    GetOutput()[i - diff] = gl_x_vec[i];

  return true;
}

bool KonovalovSSeidelMethodMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
