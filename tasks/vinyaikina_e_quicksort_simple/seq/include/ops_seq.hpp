#pragma once

#include "vinyaikina_e_quicksort_simple/common/include/common.hpp"
#include "task/include/task.hpp"

namespace vinyaikina_e_quicksort_simple {

class VinyaikinaEQuicksortSimpleSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit VinyaikinaEQuicksortSimpleSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace vinyaikina_e_quicksort_simple
