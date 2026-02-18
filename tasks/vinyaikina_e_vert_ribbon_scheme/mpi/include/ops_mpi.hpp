#pragma once

#include "vinyaikina_e_vert_ribbon_scheme/common/include/common.hpp"
#include "task/include/task.hpp"

namespace vinyaikina_e_vert_ribbon_scheme {

class VinyaikinaEVertRibbonSchemeMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit VinyaikinaEVertRibbonSchemeMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace vinyaikina_e_vert_ribbon_scheme
