#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace konovalov_s_simpson_method {

using InType = std::tuple<double, double, int>;
using OutType = double;
using TestType = std::tuple<double, double, int>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace konovalov_s_simpson_method
