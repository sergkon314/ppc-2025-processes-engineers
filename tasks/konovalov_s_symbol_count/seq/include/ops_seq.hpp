#pragma once

#include "konovalov_s_symbol_count/common/include/common.hpp"
#include "task/include/task.hpp"

namespace konovalov_s_symbol_count {

class KonovalovSSymbolCountSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit KonovalovSSymbolCountSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace konovalov_s_symbol_count
