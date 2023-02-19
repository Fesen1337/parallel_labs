#include <mpi.h>
#include <stdio.h>

#define VEC_SIZE 15

int main (int argc, char* argv[]) {

	int rank, value = 0;
  int array[15];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i < VEC_SIZE; i++)
    array[i] = rank;
  
  if (rank != 0) {
    char buffer[1024];
    char tmp_buf[1024];
    snprintf(buffer, 1024, "Init data for process %d: \n", rank);
    for(int i = 0; i < VEC_SIZE; i++) {
      snprintf(tmp_buf, 1024, "%d ", array[i]);
      strcat(buffer, tmp_buf);
    }
    strcat(buffer, "\n\n");
    printf("%s", buffer);

  }else {
    char buffer[1024];
    char tmp_buf[1024];
    snprintf(buffer, 1024, "Init data for Main_process: \n");
    for(int i = 0; i < VEC_SIZE; i++) {
      snprintf(tmp_buf, 1024, "%d ", array[i]);
      strcat(buffer, tmp_buf);
    }
    strcat(buffer, "\n\n");
    printf("%s", buffer);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  double start = MPI_Wtime();
  MPI_Bcast(array, VEC_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  double end = MPI_Wtime();

  if (rank != 0) {
    char buffer[1024];
    char tmp_buf[1024];
    snprintf(buffer, 1024, "Recieved data for process %d: \n", rank);
    for(int i = 0; i < VEC_SIZE; i++) {
      snprintf(tmp_buf, 1024, "%d ", array[i]);
      strcat(buffer, tmp_buf);
    }
    snprintf(tmp_buf, 1024, "\nTime=%lf\n\n", end-start);
    strcat(buffer, tmp_buf);
    printf("%s", buffer);
  }
  MPI_Finalize();

	return 0;
}