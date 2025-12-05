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
  return true;
}

bool KonovalovSSeidelMethodMPI::PreProcessingImpl() {
  return true;
}

bool KonovalovSSeidelMethodMPI::RunImpl() {
 return true;
}

bool KonovalovSSeidelMethodMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_seidel_iterative_method
