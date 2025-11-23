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
  std::size_t line_length = line.length();
  
  int symbols_count = 0;

  for (std::uint32_t i = 0; i < line_length; i++){
      if(isalnum(line[i]) && !isdigit(line[i])) symbols_count++;
  }

  //GetOutput() = static_cast<OutType>(symbols_count);

  GetOutput() = static_cast<OutType>(symbols_count);

  return true;
}

bool KonovalovSSymbolCountSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
