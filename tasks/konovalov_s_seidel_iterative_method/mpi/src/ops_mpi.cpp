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

std::vector<double> KonovalovSSeidelMethodMPI::IterationProcess(int mtr_size, int diff, int rank, std::vector<int> &_A,
                                                                std::vector<int> &_B, std::vector<int> X_gl) {
  // auto it = std::find_if_not(a.begin(), a.end(), iszero());
  // int d = distance(_A.begin(), it);
  std::vector<double> X = X_gl;
  std::vector<double> X_ret(diff, 0.0);

  // while (iter != 0) {
  //   bool flag = true;

  std::vector<double> X_new(mtr_size, 0.0);
  for (int i = 0; i < diff; i++) {
    int diag = (rank - 1) * (diff) + i;
    X_new[diag] = (double)_B[i] / (double)_A[diag + i * mtr_size];
    for (int j = 0; j < mtr_size; j++) {
      if (i * mtr_size + j == diag) {
        continue;
      }
      X_new[diag] -= ((double)_A[i * mtr_size + j] / (double)_A[diag + i * mtr_size]) * X[j];
    }

    // flag = flag && (std::fabs(X_new[diag] - X[diag]) < epsi);

    // std::cout << __FILE__ << ":" << __LINE__ << ": r: " << rank << " iter: " << iter << " daig:" << diag
    //           << " B[i]: " << _B[i] << " A: " << _A[diag + i * mtr_size];
    // std::cout << " old: " << X[diag] << " stored: " << X_new[diag] << std::endl;
    X[diag] = round(X_new[diag] * 1000) / 1000;
    X_ret[i] = X[diag];
    // MPI_Barrier(MPI_COMM_WORLD);
  }
  // iter--;

  // if (flag) {
  //   break;
  // }
  // }
  return X_ret;
}

bool KonovalovSSeidelMethodMPI::RunImpl() {
  srand(time(NULL));

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int matrix_size = GetInput();
  int diff = matrix_size / (size - 1);
  std::vector<double> gl_x_vec(size*diff, 0.0);
  std::vector<double> local_x_vec(diff, 0.0);
  std::vector<int> colors(matrix_size);
  std::vector<int> A_local;
  std::vector<int> B_local;

  if (rank == 0) {
    std::vector<int> A;
    std::vector<int> B;
    gl_x_vec.resize(matrix_size, 0.0);
    colors.resize(matrix_size, 0);
    B.resize(matrix_size, 0);
    A.resize(matrix_size * matrix_size, 0);

    InitMatrixA(matrix_size, 10, A, matrix_size / (size - 1));
    InitMatrixB(matrix_size, 10, B);
    colors = Coloring(matrix_size, A);

    std::cout << "mtr size:  " << matrix_size << std::endl;

    // MPI_Bcast(&matrix_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "p" << rank << " B: " << std::endl;
    for (int i = 0; i < matrix_size; i++) {
      std::cout << B[i] << " ";
    }
    std::cout << std::endl;
    // int *ndisplacement = new int[size];
    // for (int i = 0; i < size; i++) {
    //   ndisplacement[i] = i * matrix_size * GetInput();
    // }
    // const int *sdisplacement = ndisplacement;

    // int *ncounts = new int[size];
    // for (int i = 0; i < size; i++) {
    //   ncounts[i] = matrix_size * GetInput();
    // }
    // const int *scounts = ncounts;

    // std::cout << "0r scounts: ";
    // for (int i = 0; i < size - 1; i++) {
    //   std::cout << scounts[i] << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "0r displ: ";
    // for (int i = 0; i < size - 1; i++) {
    //   std::cout << sdisplacement[i] << " ";
    // }
    // std::cout << std::endl;

    // MPI_Scatterv(&A, scounts, sdisplacement, MPI_INT, &A_local, matrix_size*GetInput(), MPI_INT, 0, MPI_COMM_WORLD);

    int r = matrix_size * diff;
    for (int i = 1; i < size; i++) {
      int p = (i - 1) * r;
      MPI_Send(&A[p], r, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&B[(i - 1) * diff], diff, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    // MPI_Status s;
    // for (int i = 1; i < size; i++) {
    //   std::vector<double> ret(matrix_size);
    //   MPI_Recv(&ret[(s.MPI_SOURCE-1)*diff], diff, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
    //     // cout << "source: " << s.MPI_SOURCE << " tag: " << s.MPI_TAG << " ret: " << ret << endl;

    // }
    // std::cout << "p" << rank << " global: " << std::endl;
    // for (int i = 0; i < matrix_size; i++) {
    //   std::cout << gl_x_vec[i] << " ";
    // }
    // std::cout << std::endl;
  }
  // MPI_Bcast(&B[0], matrix_size, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank != 0) {
    MPI_Status s;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &s);
    int n;
    MPI_Get_count(&s, MPI_CHAR, &n);
    A_local.resize(matrix_size * diff);
    B_local.resize(diff);

    MPI_Recv(&A_local[0], matrix_size * diff, MPI_INT, 0, 0, MPI_COMM_WORLD, &s);
    MPI_Recv(&B_local[0], diff, MPI_INT, 0, 0, MPI_COMM_WORLD, &s);
    // std::cout << "size: " << A_local.size() << std::endl;
    // std::cout << "mtr size:  " << matrix_size << std::endl;
    // std::cout << "p" << rank << " local A: " << std::endl;
    // int c = 0;
    // for (int i = 0; i < matrix_size * diff; i++) {
    //   std::cout << A_local[i] << " ";
    //   if (i % matrix_size == matrix_size - 1) {
    //     std::cout << B_local[c] << std::endl;
    //     c++;
    //   }
    // }
    // std::cout << std::endl;
    local_x_vec = IterationProcess(matrix_size, diff, rank, A_local, B_local);
    std::cout << "p" << rank << " local X: " << std::endl;
    for (int i = 0; i < diff; i++) {
      std::cout << local_x_vec[i] << " ";
    }
    std::cout << std::endl;
    // MPI_Send(&local_x_vec[(rank - 1) * diff], diff, MPI_INT, s.MPI_SOURCE, rank, MPI_COMM_WORLD);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::vector<int> counts(size, diff);
    
    std::cout << "p" << rank << " counts: " << counts.data() << std::endl;

    std::vector<int> displacements(size, size);
    
    for (int i = 0; i < size; i++) {
      displacements[i] = diff*i;
    }

    std::cout << "p" << rank << " displ: " ;//<< displacements.data() << std::endl;
    for (int i = 0; i < size; i++) {
      std::cout << displacements[i] << " ";
    }
    std::cout << std::endl;

    MPI_Gatherv(&local_x_vec[0], diff, MPI_DOUBLE, &gl_x_vec[0], counts.data(), displacements.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::cout << std::endl;
    std::cout << "p" << rank << " gl X: " << std::endl;
    for (int i = 0; i < size*diff; i++) {
      std::cout << gl_x_vec[i] << " ";
    }
    std::cout << std::endl;
  } else 
  MPI_Gatherv(&local_x_vec[0], diff, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&gl_x_vec[0], size*diff, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  for(int i = diff; i < size*diff; i++)
    GetOutput()[i-diff] = gl_x_vec[i];
  // GetOutput() = gl_x_vec;

  return true;
}

bool KonovalovSSeidelMethodMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
