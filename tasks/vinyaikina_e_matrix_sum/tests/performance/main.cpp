#include <gtest/gtest.h>

#include "vinyaikina_e_matrix_sum/common/include/common.hpp"
#include "vinyaikina_e_matrix_sum/mpi/include/ops_mpi.hpp"
#include "vinyaikina_e_matrix_sum/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace vinyaikina_e_matrix_sum {

class VinyaikinaEMatrixSumPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};

  void SetUp() override {
    input_data_ = kCount_;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return input_data_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(VinyaikinaEMatrixSumPerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, VinyaikinaEMatrixSumMPI, VinyaikinaEMatrixSumSEQ>(PPC_SETTINGS_vinyaikina_e_matrix_sum);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = VinyaikinaEMatrixSumPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, VinyaikinaEMatrixSumPerfTests, kGtestValues, kPerfTestName);

}  // namespace vinyaikina_e_matrix_sum
