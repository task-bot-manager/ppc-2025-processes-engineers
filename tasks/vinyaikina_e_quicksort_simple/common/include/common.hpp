#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace vinyaikina_e_quicksort_simple {

using InType = int;
using OutType = int;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace vinyaikina_e_quicksort_simple
