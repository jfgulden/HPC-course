#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=outputs/setp10-outputs/exam-out.%j
#SBATCH --error=outputs/setp10-outputs/exam-err.%j
#SBATCH --time=00:30:00
#SBATCH --partition=batch
#SBATCH --account=training2427

module purge
module load GCC ParaStationMPI

echo $USER
date

#serial execution
echo ""
echo "- SERIAL EXECUTION"
./poly-ser

#mpi execution
echo ""
echo "- MPI EXECUTION"
for ranks in 2 4 8 16 32 64 128
do
    srun --ntasks=$ranks ./poly-mpi
done

#omp execution
echo ""
echo "- OMP EXECUTION"
for threads in 2 4 8 16 32 64 128
do
    export OMP_NUM_THREADS=$threads
    ./poly-omp
done

#hybrid execution
echo ""
echo "- HYBRID EXECUTION"
for ranks in 2 4 8 16 32 64 128
do
    for threads in 2 4 8 16 32 64 128
    do
        export OMP_NUM_THREADS=$threads
        srun --ntasks=$ranks --cpus-per-task=$threads ./poly-hyb
    done
done







