#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int myrank, numprocs;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    //Exercise: Fix hello-mpi so that number of ranks is printed first
    if (myrank == 0){
        printf("number of processes: %i\n", numprocs);
        fflush(stdout); 
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("hello from %d\n", myrank);
    MPI_Finalize();
}