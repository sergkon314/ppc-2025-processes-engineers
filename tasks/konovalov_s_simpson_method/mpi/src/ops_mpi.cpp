#include "konovalov_s_simpson_method/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "konovalov_s_simpson_method/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_simpson_method {

KonovalovSSimpsonMethodMPI::KonovalovSSimpsonMethodMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KonovalovSSimpsonMethodMPI::ValidationImpl() {
  return true;
}

bool KonovalovSSimpsonMethodMPI::PreProcessingImpl() {
  return true;
}

bool KonovalovSSimpsonMethodMPI::RunImpl() {
  return true;
}

bool KonovalovSSimpsonMethodMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_simpson_method
