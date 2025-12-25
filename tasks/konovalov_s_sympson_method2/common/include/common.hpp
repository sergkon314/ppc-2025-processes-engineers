#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include "task/include/task.hpp"

namespace konovalov_s_sympson_method2 {

using InType = std::tuple<double, double, double, double, int>;
using OutType = double;
using TestType = std::tuple<double, double, double, double, int>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace konovalov_s_sympson_method2