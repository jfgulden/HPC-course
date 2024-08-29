#include <stdio.h>
#include <float.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int    i, myrank, numprocs, numthreads, thread_num;
    double ymax, lmax, step, coeff[4], domain[3];
    FILE* in;
    double starttime, endtime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    
    if ( myrank == 0 ) {
        starttime = MPI_Wtime();
        if ( (in = fopen("poly.dat", "r")) == NULL ) {
            perror("poly.dat");
            return 1;
        }
        fscanf(in, "%lg %lg %lg %lg", &coeff[0], &coeff[1], &coeff[2], &coeff[3]);
        fscanf(in, "%lg %lg %lg", &domain[0], &domain[1], &domain[2]);
    }

    MPI_Bcast(coeff, 4, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(domain, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    lmax = -DBL_MAX;
    step = (domain[1] - domain[0]) / (domain[2] - 1);

    
    #pragma omp parallel
    {
        thread_num = omp_get_thread_num();
        if (thread_num == 0){
            numthreads = omp_get_num_threads();            
        }
        #pragma omp for reduction(max:ymax)
        for (i=myrank; i<(int)domain[2]; i+=numprocs) {
            double x = domain[0] + i * step;
            double y = coeff[3]*x*x*x + coeff[2]*x*x + coeff[1]*x + coeff[0];
            if ( y > lmax ) lmax = y;
        }
    }

    MPI_Reduce(&lmax, &ymax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if ( myrank == 0 ) {
        printf("Maximum is %lg\n", ymax);
        endtime = MPI_Wtime();
        printf("HYB %i ranks %i threads used %.6f seconds\n", numprocs, numthreads, endtime-starttime);
    }
    MPI_Finalize();
    return 0;
}
