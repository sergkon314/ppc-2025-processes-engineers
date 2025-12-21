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
  
  int rank, size, str_len, rad;
  int count = 0;
  int gl_non_digit_count = 0;
  std::string line;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  auto start = std::chrono::steady_clock::now();
  if (rank == 0) {
    line = GetInput();
    str_len = line.length();
    rad = round(str_len / size);
    // std::cout << __FILE__ << ":" << __LINE__ << ": " << rad << " " << str_len << " " << line << std::endl;
  }
  MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if(rank != 0) line.resize(str_len);
  MPI_Bcast(line.data(), str_len, MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(&rad, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  
  // std::cout << __FILE__ << ":" << __LINE__ << ": chunk: " << rad << " len: " << str_len << " line: " << line << " rank: " << rank << std::endl;
  
  if(rank != size - 1){
    for(int i = rad*rank; i < rad*(rank + 1); i++){
      if(isalpha(line[i])) count++;
    }
  }
  else{
    for(int i = rad*rank; i < str_len; i++){
      if(isalpha(line[i])) count++;
    }
  }
  
  
  MPI_Allreduce(&count, &gl_non_digit_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
  GetOutput() = static_cast<OutType>(gl_non_digit_count);
  auto end = std::chrono::steady_clock::now();
  if(rank == 0){
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << __FILE__ << ":" << __LINE__ << ": " << duration.count() <<  std::endl;
  }
  return true;
}

bool KonovalovSSymbolCountMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
