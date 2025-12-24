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
  
  int count = count_if(line.begin(), line.end(), [](unsigned char c) { return isdigit(c) == 0; });
  GetOutput() = static_cast<OutType>(count);

  return true;
}

bool KonovalovSSymbolCountSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
