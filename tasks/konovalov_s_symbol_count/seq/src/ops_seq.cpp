#include "konovalov_s_symbol_count/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "util/include/util.hpp"

namespace konovalov_s_symbol_count {

KonovalovSSymbolCountSEQ::KonovalovSSymbolCountSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::make_tuple(0, 0);
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
      if(isalpha(line[i])) symbols_count++;
  }

  //GetOutput() = static_cast<OutType>(symbols_count);

  GetOutput() = static_cast<OutType>(std::make_tuple(symbols_count, 0));

  return true;
}

bool KonovalovSSymbolCountSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace konovalov_s_symbol_count
