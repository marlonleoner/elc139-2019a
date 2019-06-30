#!/bin/bash
#SBATCH -J T9Cuda            # Job name
#SBATCH -o T9Cuda%j.out      # Output filename
#SBATCH -e T9Cuda%j.err      # Error filename
#SBATCH --nodes 1            # Total nodes
#SBATCH --nodelist cder01    # Node
#SBATCH --ntasks-per-node 1  # Cores per node.
#SBATCH -p qCDER             # Partition --qCDER
#SBATCH --gres=gpu:1         # GPUs
#SBATCH -t 08:00:00          # Runtime - 08 hours

module load Cuda7.0

compile="make"
echo " > [Makefile] Compiling..."
eval "$compile"
echo " > [Makefile] Done!"

run="make run"
echo " > [Makefile] Running..."
eval "$run"
echo " > [Makefile] Done!" 