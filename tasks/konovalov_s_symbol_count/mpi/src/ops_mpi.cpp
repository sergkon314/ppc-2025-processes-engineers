#include "konovalov_s_symbol_count/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_symbol_count {

KonovalovSSymbolCountMPI::KonovalovSSymbolCountMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KonovalovSSymbolCountMPI::ValidationImpl() {
  return true;
}

bool KonovalovSSymbolCountMPI::PreProcessingImpl() {
  return true;
}

bool KonovalovSSymbolCountMPI::RunImpl() {
  InType &line = GetInput();
  std::size_t line_length = line.length();
  
  int rank = 0;
  int size = 1;

  MPI_COMM_rank(MPI_COMM_WORLD, &rank);
  MPI_COMM_size(MPI_COMM_WORLD, &size);

  int process_line_lenth = line_length/size;

  int process_symbols_count = 0;

  for (std::uint32_t i = rank; i < process_line_lenth*rank; i++){
      if(isalnum(line[i]) && !isdigit(line[i])) process_symbols_count++;
  }

  int general_symbols_count = 0;

  MPI_Bcast(&general_symbols_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Reduce(&general_symbols_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  return true;
}

bool KonovalovSSymbolCountMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
