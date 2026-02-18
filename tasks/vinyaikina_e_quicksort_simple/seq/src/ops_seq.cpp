#include "vinyaikina_e_quicksort_simple/seq/include/ops_seq.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace vinyaikina_e_quicksort_simple {

namespace {

void QuickSort(std::vector<int> &arr, int left, int right) {
  if (left >= right) {
    return;
  }
  int pivot = arr[left + (right - left) / 2];
  int i = left;
  int j = right;
  while (i <= j) {
    while (arr[i] < pivot) {
      i++;
    }
    while (arr[j] > pivot) {
      j--;
    }
    if (i <= j) {
      std::swap(arr[i], arr[j]);
      i++;
      j--;
    }
  }
  if (left < j) {
    QuickSort(arr, left, j);
  }
  if (i < right) {
    QuickSort(arr, i, right);
  }
}

}  // namespace

VinyaikinaEQuicksortSimpleSEQ::VinyaikinaEQuicksortSimpleSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
}

bool VinyaikinaEQuicksortSimpleSEQ::ValidationImpl() {
  return true;
}

bool VinyaikinaEQuicksortSimpleSEQ::PreProcessingImpl() {
  data_ = GetInput();
  return true;
}

bool VinyaikinaEQuicksortSimpleSEQ::RunImpl() {
  if (data_.size() > 1) {
    QuickSort(data_, 0, static_cast<int>(data_.size()) - 1);
  }
  return true;
}

bool VinyaikinaEQuicksortSimpleSEQ::PostProcessingImpl() {
  GetOutput() = data_;
  return true;
}

}  // namespace vinyaikina_e_quicksort_simple
