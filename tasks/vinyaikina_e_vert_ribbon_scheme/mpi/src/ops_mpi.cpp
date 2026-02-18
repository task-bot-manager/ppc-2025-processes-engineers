#include "vinyaikina_e_vert_ribbon_scheme/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <cstddef>
#include <vector>

#include "vinyaikina_e_vert_ribbon_scheme/common/include/common.hpp"

namespace vinyaikina_e_vert_ribbon_scheme {

namespace {

void ComputeColumnDistribution(int cols, int num_proc, std::vector<int> &counts, std::vector<int> &starts) {
  int base = cols / num_proc;
  int rem = cols % num_proc;
  for (int i = 0; i < num_proc; i++) {
    counts[i] = base + (i < rem ? 1 : 0);
  }
  starts[0] = 0;
  for (int i = 1; i < num_proc; i++) {
    starts[i] = starts[i - 1] + counts[i - 1];
  }
}

std::vector<int> PackSendBuffer(int rows, int cols, int num_proc, const std::vector<int> &matrix,
                                const std::vector<int> &col_counts, const std::vector<int> &col_starts,
                                std::vector<int> &send_counts, std::vector<int> &send_offsets) {
  int total_send = 0;
  for (int pr = 0; pr < num_proc; pr++) {
    send_counts[pr] = rows * col_counts[pr];
    send_offsets[pr] = total_send;
    total_send += send_counts[pr];
  }
  std::vector<int> buf(total_send);
  for (int pr = 0; pr < num_proc; pr++) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < col_counts[pr]; j++) {
        buf[send_offsets[pr] + (i * col_counts[pr]) + j] = matrix[(i * cols) + col_starts[pr] + j];
      }
    }
  }
  return buf;
}

}  // namespace

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
    matrix_.assign(static_cast<std::size_t>(rows_) * cols_, 1);
  }
  row_sums_.assign(rows_, 0);
  return true;
}

bool VinyaikinaEVertRibbonSchemeMPI::RunImpl() {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<int> col_counts(size);
  std::vector<int> col_starts(size);
  ComputeColumnDistribution(cols_, size, col_counts, col_starts);

  int my_cols = col_counts[rank];

  std::vector<int> send_buf;
  std::vector<int> send_counts(size, 0);
  std::vector<int> send_offsets(size, 0);

  if (rank == 0) {
    send_buf = PackSendBuffer(rows_, cols_, size, matrix_, col_counts, col_starts, send_counts, send_offsets);
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
    for (int dest = 1; dest < size; dest++) {
      MPI_Send(&total, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
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
