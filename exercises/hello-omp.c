#include <stdio.h>
#include <omp.h>


int main(){
    int myid, nthreads;

    #pragma omp parallel private(myid, nthreads)
    {
        myid = omp_get_thread_num();
        if (myid == 0){
            nthreads = omp_get_num_threads();
            printf("Total number of threads: %d\n", nthreads);
        }
        printf("hello world from %d\n", myid);
    }
}