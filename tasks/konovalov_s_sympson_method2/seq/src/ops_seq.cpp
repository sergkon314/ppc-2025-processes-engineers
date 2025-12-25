#include "konovalov_s_sympson_method2/seq/include/ops_seq.hpp"

#include <cstddef>
#include <vector>

#include "konovalov_s_sympson_method2/common/include/common.hpp"

namespace konovalov_s_sympson_method2 {

KonovalovSSympsonMethodSEQ::KonovalovSSympsonMethodSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0.0;
}

bool KonovalovSSympsonMethodSEQ::ValidationImpl() {
  return GetInput() != std::make_tuple(0.0, 0.0, 0.0, 0.0, 0) && std::get<4>(GetInput()) % 2 == 0;
}

bool KonovalovSSympsonMethodSEQ::PreProcessingImpl() {
  return true;
}

double KonovalovSSympsonMethodSEQ::GetFunc(double x, double y) {
  return (double)(rand())/RAND_MAX*(10) + std::max(x, y);
}

double KonovalovSSympsonMethodSEQ::SimpsonMethod(double a, double b, double c, double d, int n) {

  double h = (b - a) / n;
  double k = (d - c) / n;

  double x, y, sum = 0;

  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= n; j++) {
      x = a + i * h;
      y = c + j * k;

      int coeff = (i == 0 || i == n) ? 1 : (i % 2 == 1 ? 4 : 2);

      sum += coeff * GetFunc(x, y);
    }
  }

  return (h * k / 9.0) * sum;
}

bool KonovalovSSympsonMethodSEQ::RunImpl() {
  GetOutput() = SimpsonMethod(std::get<0>(GetInput()), std::get<1>(GetInput()), std::get<2>(GetInput()),
                                std::get<3>(GetInput()), std::get<4>(GetInput()));
  return true;
}

bool KonovalovSSympsonMethodSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_sympson_method2
