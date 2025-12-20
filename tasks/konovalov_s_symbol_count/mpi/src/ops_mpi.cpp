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
  GetOutput() = std::make_tuple((int)0, (int)0);
}

bool KonovalovSSymbolCountMPI::ValidationImpl() {
  return true;
}

bool KonovalovSSymbolCountMPI::PreProcessingImpl() {
  return true;
}

bool KonovalovSSymbolCountMPI::RunImpl() {
  // int rank = 0;
  // int size = 0;
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &size);
  // MPI_Status s;

  // std::string line = GetInput();
  // std::vector<int> local_result(size, 0);
  // int local_line_len = int((line.size()/(size - 2)));
  // int tail = line.size()%(size - 2);
  // std::string tmp;
  // if(rank == 0){
  //   for (int i = 1; i < size; i++) {
  //     if(i != size - 1) MPI_Send(&line[(i-1)*local_line_len], local_line_len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
  //     else MPI_Send(&line[(i-1)*local_line_len], tail, MPI_CHAR, i, 0, MPI_COMM_WORLD);
  //   }
  //   for (int i = 1; i < size; i++){
  //     MPI_Recv(&local_result[i], 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
  //   }
  //   GetOutput() = static_cast<OutType>(std::accumulate(local_result.begin(), local_result.end(), 0));
  // } else {
    //   int x = 0;
    //   if(rank != (size - 1)) MPI_Recv(&tmp[0], local_line_len, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
    //   else MPI_Recv(&tmp[0], tail, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
    //   for (long unsigned int i = 0; i < tmp.size(); i++) if (isalpha(tmp[i])) x++;
    //   MPI_Send(&x, sizeof(MPI_INT), MPI_INT, 0, 0, MPI_COMM_WORLD);
    // }
    
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mpi_non_digit_count = 0;
    std::vector<int> vret;
    
  if (rank == 0) {
    std::string str = GetInput();

    // int non_digit_count = count_if(str.begin(), str.end(), [](unsigned char c) {
    //   return isdigit(c) == 0; });

    const char *c_str = str.data();
    size_t len = strlen(c_str);
    int r = round(len / (size - 1));

    for (int i = 1; i < size; i++) {
      int p = (i - 1) * r;
      if (i == size - 1)
        MPI_Send(&c_str[p], len - p, MPI_CHAR, i, 0, MPI_COMM_WORLD);
      else
        MPI_Send(&c_str[p], r, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }

    MPI_Status s;
    for (int i = 1; i < size; i++) {
      int ret;
      MPI_Recv(&ret, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
      std::cout << __FILE__ << ":" << __LINE__ << " ret: " << ret << " " << s.MPI_SOURCE << std::endl;
      vret.push_back(ret);
      mpi_non_digit_count += ret;
    }
    std::cout << __FILE__ << ":" << __LINE__ << ": " << mpi_non_digit_count << std::endl;
    // GetOutput() = static_cast<OutType>(mpi_non_digit_count);

  } else {
    MPI_Status s;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &s);
    int n;
    MPI_Get_count(&s, MPI_CHAR, &n);

    std::vector<char> a(n);
    MPI_Recv(&a[0], n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &s);
    std::cout << "rank: " << rank << " recv: (" << n << ") ";
    for (auto i : a) {
      std::cout << i;
    }
    std::cout << std::endl;
    int count = count_if(a.begin(), a.end(), [](unsigned char c) {
      return isdigit(c) == 0; });

    // MPI_Allreduce(&count, &mpi_non_digit_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Send(&count, 1, MPI_INT, s.MPI_SOURCE, rank, MPI_COMM_WORLD);
  }

  // if(rank == 0){
    GetOutput() = static_cast<OutType>(std::make_tuple(mpi_non_digit_count, rank));
  //   for(int i = 1; i < size; i++){
  //     MPI_Send(&mpi_non_digit_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
  //   }
  // }
  // else{
  //   MPI_Status s;
  //   int res = 0;
  //   MPI_Recv(&res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &s);
  //   GetOutput() = res;
  // }
  // MPI_Barrier(MPI_COMM_WORLD);
  return true;
}

bool KonovalovSSymbolCountMPI::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
