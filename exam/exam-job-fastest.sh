#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=outputs/exam-opt-out.%j
#SBATCH --error=outputs/exam-opt-err.%j
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

#mpi execution (128 ranks)
echo ""
echo "- MPI EXECUTION"

srun --ntasks=128 ./poly-mpi


#omp execution (64 threads)
echo ""
echo "- OMP EXECUTION"

export OMP_NUM_THREADS=64
./poly-omp


#hybrid execution (64 ranks and 2 threads)
echo ""
echo "- HYBRID EXECUTION"

export OMP_NUM_THREADS=2
srun --ntasks=64 --cpus-per-task=2 ./poly-hyb




