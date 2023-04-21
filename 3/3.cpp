#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <mpi.h>

#define ROWS 4
#define COLS 5

void swap(char* a, char* b) {
  char tmp = *a;
  *a = *b;
  *b = tmp;
}
void sort(char* mass, size_t size) {
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size - 1 - i; j++) {
      if (*(mass + j) > *(mass + j + 1))
        swap(mass + j, mass + j + 1);
    }
  }
}

int main(int argc, char* argv[])
{
  char vec[ROWS][COLS];
  int numprocs, myid, using_proc_count, ost = 0;
  char* sorted_buf;
  int *send_counts, *shift;
  srand(time(NULL));
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  if (myid == 0) {
    printf("Init matrix:\n");
    for (size_t i = 0; i < ROWS; i++) {
      for (size_t j = 0; j < COLS; j++) {
        vec[i][j] = rand() % 20;
        printf("%d ", vec[i][j]);
      }
      printf("\n");
    }
    printf("\n\n");
  }
  if (ROWS >= numprocs) {
    ost = ROWS % numprocs;
    using_proc_count = numprocs;
  } else {
    using_proc_count = ROWS;
  }

  if (myid < using_proc_count) {
    send_counts = (int*)calloc(numprocs, sizeof(int));
    shift = (int*)malloc(numprocs * sizeof(int));
    for (size_t i = 0; i < using_proc_count; i++) {
      if (ROWS >= numprocs) {
        send_counts[i] = ROWS / numprocs * COLS;
      } else {
        send_counts[i] = COLS;
      }
      if (i > 0 && ost > 0)
        if (i <= ost) shift[i] = i * send_counts[i] + i * COLS;
        else shift[i] = i * send_counts[i] + ost * COLS;
      else shift[i] = i * send_counts[i];
      if (i < ost) send_counts[i] += COLS;
    }
    sorted_buf = (char*)malloc(send_counts[myid] * sizeof(char));
    MPI_Scatterv(vec, send_counts, shift,
    MPI_CHAR, sorted_buf, send_counts[myid],
    MPI_CHAR, 0, MPI_COMM_WORLD);
    for (size_t i = 0; i < (*send_counts); i += COLS) {
      sort(sorted_buf + i, COLS);
    }
    MPI_Gatherv(sorted_buf, send_counts[myid], MPI_CHAR, vec,
        send_counts, shift, MPI_CHAR, 0, MPI_COMM_WORLD);
    free(sorted_buf);
    free(send_counts);
    free(shift);
  }
  if (myid == 0) {
    printf("Sorted matrix:\n");
    for (size_t i = 0; i < ROWS; i++) {
      for (size_t j = 0; j < COLS; j++)
        printf("%d ", vec[i][j]);
      printf("\n");
    }
    printf("\n");
  }
  MPI_Finalize();
}