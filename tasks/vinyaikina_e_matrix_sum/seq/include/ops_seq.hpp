#pragma once

#include "vinyaikina_e_matrix_sum/common/include/common.hpp"
#include "task/include/task.hpp"

namespace vinyaikina_e_matrix_sum {

class VinyaikinaEMatrixSumSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit VinyaikinaEMatrixSumSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace vinyaikina_e_matrix_sum
