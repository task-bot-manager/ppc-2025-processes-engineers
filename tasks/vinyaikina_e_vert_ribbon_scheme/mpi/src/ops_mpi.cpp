#include "vinyaikina_e_vert_ribbon_scheme/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <vector>

namespace vinyaikina_e_vert_ribbon_scheme {

VinyaikinaEVertRibbonSchemeMPI::VinyaikinaEVertRibbonSchemeMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool VinyaikinaEVertRibbonSchemeMPI::ValidationImpl() {
  return GetInput() > 0 && GetOutput() == 0;
}

bool VinyaikinaEVertRibbonSchemeMPI::PreProcessingImpl() {
  rows_ = GetInput();
  cols_ = GetInput();
  if (rows_ <= 0 || cols_ <= 0) {
    return false;
  }
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matrix_.assign(static_cast<size_t>(rows_) * cols_, 1);
  }
  row_sums_.assign(rows_, 0);
  return true;
}

bool VinyaikinaEVertRibbonSchemeMPI::RunImpl() {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int base_cols = cols_ / size;
  int extra = cols_ % size;

  std::vector<int> col_counts(size);
  std::vector<int> col_starts(size);
  for (int i = 0; i < size; i++) {
    col_counts[i] = base_cols + (i < extra ? 1 : 0);
  }
  col_starts[0] = 0;
  for (int i = 1; i < size; i++) {
    col_starts[i] = col_starts[i - 1] + col_counts[i - 1];
  }

  int my_cols = col_counts[rank];

  std::vector<int> send_buf;
  std::vector<int> send_counts(size, 0);
  std::vector<int> send_offsets(size, 0);

  if (rank == 0) {
    int total_send = 0;
    for (int p = 0; p < size; p++) {
      send_counts[p] = rows_ * col_counts[p];
      send_offsets[p] = total_send;
      total_send += send_counts[p];
    }
    send_buf.resize(total_send);
    for (int p = 0; p < size; p++) {
      for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < col_counts[p]; j++) {
          send_buf[send_offsets[p] + (i * col_counts[p]) + j] = matrix_[(i * cols_) + col_starts[p] + j];
        }
      }
    }
  }

  int recv_count = rows_ * my_cols;
  std::vector<int> local_data(recv_count);
  MPI_Scatterv(send_buf.data(), send_counts.data(), send_offsets.data(), MPI_INT, local_data.data(), recv_count,
               MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> local_sums(rows_, 0);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < my_cols; j++) {
      local_sums[i] += local_data[(i * my_cols) + j];
    }
  }

  row_sums_.assign(rows_, 0);
  MPI_Reduce(local_sums.data(), row_sums_.data(), rows_, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  int total = 0;
  if (rank == 0) {
    for (int i = 0; i < rows_; i++) {
      total += row_sums_[i];
    }
    for (int p = 1; p < size; p++) {
      MPI_Send(&total, 1, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  GetOutput() = total;
  return true;
}

bool VinyaikinaEVertRibbonSchemeMPI::PostProcessingImpl() {
  if (GetInput() == 0) {
    return false;
  }
  GetOutput() /= GetInput();
  return true;
}

}  // namespace vinyaikina_e_vert_ribbon_scheme
