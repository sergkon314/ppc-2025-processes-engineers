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
  int diff = 2;
  double x = 1 + rand() % fmax;
  for (long unsigned int i = 0; i < size; i++) {
    for(int j = 0; j < diff; j++) {
      if (i * size + i - j > 0 && i * size + i - j >= i * size) {
        A[i * size + i - j] = x * (diff - j);
      }
      if (i * size + i + j < size*size && i + j < size) {
        A[i * size + i + j] = x * (diff - j);
      }
    }
    A[i * size + i] = x * (diff*diff);
  }
}

void InitMatrixB(long unsigned int size, int fmax, std::vector<double> &B) {
  for (long unsigned int i = 0; i < size; i++) {
    B[i] = 1 + rand() % fmax;
  }
}

void coloring(int N, std::vector<double> _A){
  std::vector<double> A = _A; //coefficient matrix.int 
  std::vector<int> color(N); //colors of rows.
  for (int i = 0; i < N; ++i) color[i] = -1;
  for (int i = 0; i < N; ++i) {
    int m = 0;
    for (int j = 0; j < i; j++) {
      if (A[i*N + j] != 0 && color[j] == m) {
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
  for (auto i : _B) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return X;
}

int main(int argc, const char *argv[]) {
  
  srand(time(NULL));
  int rank = 0;
  int size = 0;

  std::vector<double> _A;
  std::vector<double> _A_local;
  std::vector<double> _B;
  std::vector<double> _B_local;

  std::string segmlist;
  std::ifstream in("sys_3.txt");
  std::vector<std::string> raw_data;

  while (std::getline(in, segmlist, ' ')) {
    raw_data.push_back(segmlist);
  }

  int _size = 8;
  int _iter = std::stoi(raw_data[1]);
  std::cout << _size << std::endl;

  _A.resize(_size * _size, 0.00);
  _A_local.resize(_size * _size, 0.00);
  _B.resize(_size, 0.00);
  _B_local.resize(_size, 0.00);

  _A.resize(_size * _size, 0.0);
  init_matrix(_size, 10, _A);
  InitMatrixB(_size, 10, _B);
  for (unsigned long int i = 0; i < _size * _size; i++) {
    std::cout << _A[i] << " ";
    if (i % _size == _size - 1) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;

  std::vector<double> X(_size, 0.0);
  X = iterationprocess(_A, _B, 0, 10, 0.001);
  coloring(_size, _A);
 for (unsigned long int i = 0; i < _size; i++) {
    std::cout << X[i] << " ";
  }
  std::cout << std::endl;
  return 0;
}
