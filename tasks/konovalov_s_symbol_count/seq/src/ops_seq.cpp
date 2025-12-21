#include "konovalov_s_symbol_count/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_symbol_count {

KonovalovSSymbolCountSEQ::KonovalovSSymbolCountSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KonovalovSSymbolCountSEQ::ValidationImpl() {
  return true;
}

bool KonovalovSSymbolCountSEQ::PreProcessingImpl() {
  return true;
}

bool KonovalovSSymbolCountSEQ::RunImpl() {
  
  InType &line = GetInput();
  
  // auto start = std::chrono::steady_clock::now();
  int count = count_if(line.begin(), line.end(), [](unsigned char c) { return isdigit(c) == 0; });
  // auto end = std::chrono::steady_clock::now();
  // std::cout << __FILE__ << ":" << __LINE__ << ": " << " res_seq: " << count << std::endl;

  //GetOutput() = static_cast<OutType>(symbols_count);

  GetOutput() = static_cast<OutType>(count);
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << __FILE__ << ":" << __LINE__ << ": " << duration.count() <<  std::endl;

  return true;
}

bool KonovalovSSymbolCountSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
