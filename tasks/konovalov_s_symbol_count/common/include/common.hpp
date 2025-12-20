#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace konovalov_s_symbol_count {

using InType = std::string;
using OutType = std::tuple<int, int>;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace konovalov_s_symbol_count
