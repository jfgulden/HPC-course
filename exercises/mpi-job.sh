#!/bin/bash
#SBATCH --nodes=4
#SBATCH --ntasks=512
#SBATCH --output=outputs/mpi-out.%j
#SBATCH --error=outputs/mpi-err.%j
#SBATCH --time=00:05:00
#SBATCH --partition=batch
#SBATCH --account=training2427
#SBATCH --reservation=intro_hpc_20240802

module purge
module load GCC ParaStationMPI
srun ./global_sum
