CC = gcc
MPICC = mpicc
OMPFLAG = -fopenmp
CFLAGS  = -Wall -Ofast -march=native

all: poly-ser-opt poly-mpi-opt poly-omp-opt poly-hyb-opt

poly-ser-opt: poly-ser.c
	$(CC) $(CFLAGS) $< -o $@

poly-mpi-opt: poly-mpi.c
	$(MPICC) $(CFLAGS) $< -o $@

poly-omp-opt: poly-omp.c
	$(CC) $(CFLAGS) $(OMPFLAG) $< -o $@

poly-hyb-opt: poly-hyb.c
	$(MPICC) $(CFLAGS) $(OMPFLAG) $< -o $@

make clean:
	rm -f poly-ser-opt poly-mpi-opt poly-omp-opt poly-hyb-opt
