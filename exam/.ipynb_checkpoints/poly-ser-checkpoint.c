#include <stdio.h>
#include <float.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int    i;
    double x, y, ymax, step, coeff[4], xmin, xmax, nsteps;
    FILE* in;

    double starttime, endtime;
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    starttime = tv.tv_sec + tv.tv_nsec * 1.0e-9;

    if ( (in = fopen("poly.dat", "r")) == NULL ) {
    perror("poly.dat");
    return 1;
    }
    fscanf(in, "%lg %lg %lg %lg", &coeff[0], &coeff[1], &coeff[2], &coeff[3]);
    fscanf(in, "%lg %lg %lg", &xmin, &xmax, &nsteps);
    
    ymax = -DBL_MAX;
    x    = xmin;
    step = (xmax - xmin) / (nsteps - 1);
    for (i=0; i<(int)nsteps; ++i) {
     y = coeff[3]*x*x*x + coeff[2]*x*x + coeff[1]*x + coeff[0];
     if ( y > ymax ) ymax = y;
     x += step;
    }
    
    printf("Maximum is %lg\n", ymax);
    clock_gettime(CLOCK_REALTIME, &tv);
    endtime = tv.tv_sec + tv.tv_nsec * 1.0e-9;
    printf("SEQ used %.6f seconds\n", endtime - starttime);
    return 0;
}
