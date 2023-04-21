#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <mpi.h>

#define M 50

int main(int argc, char* argv[])
{
  short int vec[M];
  int numprocs, myid;
  short int* res_vec = NULL;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  if (myid == 0) {
    res_vec = (short int*)malloc(M * sizeof(short int));
  }
  for (size_t i = 0; i < M; i++) {
    vec[i] = myid;//myid + rand() % 20;
  }
  printf("Process %d init vector:\n", myid);
  for (size_t i = 0; i < M; i++) printf("%d ", vec[i]);
    printf("\n\n");
    MPI_Reduce(vec, res_vec, M, MPI_LONG, MPI_PROD, 0, MPI_COMM_WORLD);
  if (myid == 0) {
    printf("Process %d result vector:\n", myid);
    for (size_t i = 0; i < M; i++) printf("%d ", res_vec[i]);
    printf("\n\n");
    free(res_vec);
  }
  MPI_Finalize();
}