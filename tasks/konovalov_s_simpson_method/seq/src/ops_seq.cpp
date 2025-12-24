#include "konovalov_s_simpson_method/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "konovalov_s_simpson_method/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_simpson_method {

KonovalovSSimpsonMethodSEQ::KonovalovSSimpsonMethodSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KonovalovSSimpsonMethodSEQ::ValidationImpl() {
  return true;
}

bool KonovalovSSimpsonMethodSEQ::PreProcessingImpl() {
  return true;
}

double KonovalovSSimpsonMethodSEQ::SquareFuncVal(double x, double y) {
  return x * x + 2 * x * y + y * y;
}
double KonovalovSSimpsonMethodSEQ::SimpsonM(double lower_lim, double upper_lim, int n) {
  double section_len = (upper_lim - lower_lim) / n;
  double res = 0;
  double x0, x1, x2;

  for (int i = 0; i < n; i++) {
    x0 = lower_lim + i * section_len;
    x2 = lower_lim + (i + 1) * section_len;
    x1 = (x0 + x2) / 2;

    res += ((x2 - x0) / 6.0) * (SquareFuncVal(x0) + SquareFuncVal(x2) + 4.0 * SquareFuncVal(x1));
  }

  return res;
}

bool KonovalovSSimpsonMethodSEQ::RunImpl() {
  int res = SimpsonM(std::get<0>(GetInput()), std::get<1>(GetInput()), std::get<2>(GetInput()));
  GetOutput() = res;
  return true;
}

bool KonovalovSSimpsonMethodSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_simpson_method
