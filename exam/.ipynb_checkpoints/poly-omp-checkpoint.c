#include <stdio.h>
#include <float.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    double coeff[4], xmin, xmax, nsteps;
    int num_threads, thread_num = 0;
    FILE* in;
    
    double starttime, endtime;
    starttime = omp_get_wtime();
    
    if ( (in = fopen("poly.dat", "r")) == NULL ) {
        perror("poly.dat");
        return 1;
    }
    fscanf(in, "%lg %lg %lg %lg", &coeff[0], &coeff[1], &coeff[2], &coeff[3]);
    fscanf(in, "%lg %lg %lg", &xmin, &xmax, &nsteps);
    
    double step = (xmax - xmin) / (nsteps - 1);
    double ymax = -DBL_MAX;
    
    #pragma omp parallel
    {
        thread_num = omp_get_thread_num();
        if (thread_num == 0){
            num_threads = omp_get_num_threads();            
        }
        #pragma omp for reduction(max:ymax)
        for (int i=0; i<(int)nsteps; ++i) {
            double x = xmin + i * step;
            double y = coeff[3]*x*x*x + coeff[2]*x*x + coeff[1]*x + coeff[0];
            if ( y > ymax) ymax = y;
        }
    }
    
    printf("Maximum is %lg\n", ymax);
    endtime = omp_get_wtime();
    printf("OMP %i threads used %.6f seconds\n", num_threads, endtime-starttime);
    return 0;
}
