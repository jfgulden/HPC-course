// Copyright (c) 2016-2021 Forschungszentrum Jülich GmbH

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void global_sum_ring(int x, int* y, int root, MPI_Comm comm) {
  int r; MPI_Comm_rank(comm, &r);
  int s; MPI_Comm_size(comm, &s);

  int next = (r + 1 < s) ? r + 1 : 0;
  int prev = (r - 1 >= 0) ? r - 1 : s - 1;
  int tag = 10;
  if (r == root) {
    // use `MPI_Sendrecv` to
    // send initial value `x` to process `next`
    // receive final result `y` from process `prev`
      
    MPI_Sendrecv(&x, 1, MPI_INT, next, tag, y, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("final result received from root: %i", *y);
  } else {
    int tmp; // partial result goes here
    // use `MPI_Recv` to receive partial result from process `prev`
    MPI_Recv(&tmp, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    tmp += x;
    MPI_Send(&tmp, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
    // use `MPI_Send` to send partial result to process `next`
  }
}

void global_sum_tree(int x, int* y, int root, MPI_Comm comm) {
  int r; MPI_Comm_rank(comm, &r);
  int s; MPI_Comm_size(comm, &s);

  int res = x;
  for (int dist = 1; dist < s; dist *= 2) {
    if (r % (2 * dist) == 0) {
      if (r + dist < s) {
        int tmp;
        // use `MPI_Recv` to receive partial result from right process
        res += tmp;
      }
    } else {
      // use `MPI_Send` to send partial result to left process
      break;
    }
  }

  MPI_Request req;
  if (r == root) {
    // use `MPI_Irecv` to initiate receive from process 0
  }

  if (r == 0) {
    // use `MPI_Send` to send final result to process `root`
  }

  if (r == root) {
    // use `MPI_Wait` to complete receive operation
  }
}

void global_sum_reduce(int x, int* y, int root, MPI_Comm comm) {
  // this can be implemented in a single call to `MPI_Reduce`
}

void test_global_sum(const char* name, void (*global_sum)(int, int*, int, MPI_Comm));

#define TEST_GLOBAL_SUM(name) test_global_sum(#name, name)

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  TEST_GLOBAL_SUM(global_sum_ring);
  //TEST_GLOBAL_SUM(global_sum_tree);
  // TEST_GLOBAL_SUM(global_sum_reduce);

  MPI_Finalize();
}

// You can ignore everything below this line

void test_global_sum(const char* name, void (*global_sum)(int, int*, int, MPI_Comm)) {
  int i, r, s, res;
  MPI_Comm_rank(MPI_COMM_WORLD, &r);
  MPI_Comm_size(MPI_COMM_WORLD, &s);

  for (i = 0; i < s; ++i) {
    global_sum(1, &res, i, MPI_COMM_WORLD);
    if (r == i && res != s) {
      fprintf(stderr,
        "'%s' failed a test\n1 + ... + 1 should be %d but is %d\n", name, s, res);
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    global_sum(r, &res, i, MPI_COMM_WORLD);
    if (r == i && res != (s * (s - 1)) / 2) {
      fprintf(stderr,
        "'%s' failed a test\n1 + ... + s - 1 should be %d but is %d\n", name, (s * (s - 1)) / 2, res);
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
  }

  if (r == 0) printf("'%s' successfully passed all tests\n", name);
}
