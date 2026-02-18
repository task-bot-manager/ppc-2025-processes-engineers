#include <gtest/gtest.h>

#include "vinyaikina_e_quicksort_simple/common/include/common.hpp"
#include "vinyaikina_e_quicksort_simple/mpi/include/ops_mpi.hpp"
#include "vinyaikina_e_quicksort_simple/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace vinyaikina_e_quicksort_simple {

class VinyaikinaEQuicksortSimplePerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
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

TEST_P(VinyaikinaEQuicksortSimplePerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, VinyaikinaEQuicksortSimpleMPI, VinyaikinaEQuicksortSimpleSEQ>(PPC_SETTINGS_vinyaikina_e_quicksort_simple);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = VinyaikinaEQuicksortSimplePerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, VinyaikinaEQuicksortSimplePerfTests, kGtestValues, kPerfTestName);

}  // namespace vinyaikina_e_quicksort_simple
