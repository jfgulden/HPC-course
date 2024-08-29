#include<stdio.h>
#include <mpi.h>

//MPI WORK DISTRIBUTION: Block solution 
double f(double i){
    return (4.0 / (1.0 + i*i));
}

int main(int argc, char* argv[]){
    int size = 0;
    int rank = 0;
    double pi = 0;
    int nstep[1];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
		FILE* in;

		if ((in = fopen("pi.dat", "r")) == NULL) {
			perror("pi.dat");
			return 1;
		}

		fscanf(in, "%d", nstep);
    }
    MPI_Bcast(nstep, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double h = 1.0 / nstep[0];
    double sum = 0.0;
    for (int i = (int) (*nstep / size * rank); i < (int) (*nstep / size * (rank+1)); i += 1){
        double x = i * h;
        sum += f(x) * h;
    }
	MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
	if (rank == 0){
		printf("Value of pi is %16.12f\n", pi);
	}
    
	MPI_Finalize();
    return 0;
}