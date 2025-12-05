#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace konovalov_s_seidel_iterative_method {

using InType = std::tuple<int, std::vector<std::vector<double>>, std::vector<double>, int>;
using OutType = std::vector<double>;
using TestType = std::string;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace konovalov_s_seidel_iterative_method
