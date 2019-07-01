#!/bin/bash
#SBATCH -J mandelbrot           # job name
#SBATCH -o mandelbrot%j.out     # output file name
#SBATCH -e mandelbrot%j.err     # error file name
#SBATCH  --nodes  1             # total number of nodes requested
#SBATCH --ntasks-per-node 1     # total number of proccess per node.
#SBATCH -p qCDER                # partition --qCDER
#SBATCH -t 01:00:00             # run time (hh:mm:ss) - 1 hour


# execute any program
compile="make"

echo " > [Compiling] $compile"
eval "$compile"
echo " > [Compiling] Done!"

run="./mandelbrot < mandelbrot.in"

echo " > [Running] $run"
eval "$run"
echo " > [Running] Done!"
