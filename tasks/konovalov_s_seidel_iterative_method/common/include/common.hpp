#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace konovalov_s_seidel_iterative_method {

using InType = int;
using OutType = std::vector<double>;
using TestType = std::tuple<const char*, int>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace konovalov_s_seidel_iterative_method
