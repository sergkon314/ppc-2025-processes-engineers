#include <mpi.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

std::vector<double> _A;
std::vector<double> _A_local;
std::vector<double> _B;
std::vector<double> _B_local;
int _size, _iter;

void init_matrix(long unsigned int size, int fmax, std::vector<double> &A) {
  double x = 1 + rand() % fmax;;
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
std::vector<double> iterationprocess(std::vector<double> &_A, std::vector<double> &_B, double X0, int _iter,
                                     double _epsi) {
  double epsi = _epsi;
  int iter = _iter;
  std::vector<double> X(_B.size(), X0);

  while (iter != 0) {
    bool flag = true;

    std::vector<double> X_new(_B.size(), 0.0);
    for (int i = 0; i < _B.size(); i++) {
      X_new[i] = _B[i] / _A[i * _B.size() + i];
      for (int j = 0; j < _B.size(); j++) {
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

  std::cout << "iter " << iter << std::endl;
  for (auto i : _A) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return X;
}

int main(int argc, const char *argv[]) {
  int rank = 0;
  int size = 0;
  // MPI_Init(&argc, (char ***)&argv);
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<double> _A;
  std::vector<double> _A_local;
  std::vector<double> _B;
  std::vector<double> _B_local;

  // MPI_Datatype custom_vec;
  std::string segmlist;
  std::ifstream in("sys_3.txt");
  std::vector<std::string> raw_data;

  while (std::getline(in, segmlist, ' ')) {
    raw_data.push_back(segmlist);
  }

  int _size = 8;
  int _iter = std::stoi(raw_data[1]);
  std::cout << _size << std::endl;

  //   std::vector<std::vector<double>> _A1(_size, std::vector<double>(_size, 0.00));
  //   std::vector<std::vector<double>> _A_local1(_size, std::vector<double>(_size, 0.00));

  _A.resize(_size * _size, 0.00);
  _A_local.resize(_size * _size, 0.00);
  _B.resize(_size, 0.00);
  _B_local.resize(_size, 0.00);

  std::vector<double> X(_size, 0.00);

  int count = 2;

  // for (int i = 0; i < _size * _size; i++) {
  //   _A[i] = std::stod(raw_data[count]);
  //   std::cout << _A[i] << " ";
  //   count++;
  // }
  // std::cout << std::endl;
  // for (int i = 0; i < _size; i++) {
  //   _B[i] = std::stod(raw_data[count]);
  //   std::cout << _B[i] << " ";
  //   count++;
  // }
  // std::cout << std::endl;
  _A.resize(_size * _size, 0.0);
  init_matrix(_size, 100, _A);
  //   MPI_Type_vector(_size, 1, _size, MPI_DOUBLE, &custom_vec);

  //   MPI_Type_commit(&custom_vec);

  // MPI_Scatter(&_A, _size, MPI_DOUBLE, &_A_local, _size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // MPI_Scatter(&_B, _size, MPI_DOUBLE, &_B_local, _size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  //   MPI_Gather()

  // std::cout << "rank " << rank << std::endl;
  for (unsigned long int i = 0; i < _size * _size; i++) {
    std::cout << _A[i] << " ";
    if (i % _size == _size - 1) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
  std::cout << _A[11] << std::endl;

  //   MPI_Type_free(&custom_vec);}

  // // MPI_Finalize();
  return 0;
}
