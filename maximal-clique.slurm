#!/bin/bash
#SBATCH --job-name=maximal-clique
#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:10:00
#SBATCH --mem=1G
#SBATCH --partition=PROVA

mpirun ./output/maximal-clique-mpi.out
