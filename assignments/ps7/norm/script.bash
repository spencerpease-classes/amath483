#!/bin/bash

trips=10

make clean


for program in cuda_norm thrust_norm lambda_norm sequential_norm stl_norm omp_norm ; do
    make ${program}.exe
    /bin/rm -f ${program}.txt
    printf "\n"
    printf "=== ${program} ===\n"
    printf "N\ttrips\ttime\n" > ${program}.txt
    for size in 20 21 22 23 24 25 26 27; do
	./${program}.exe ${size} ${trips} | tee -a ${program}.txt

    done
done

python plot.py cuda_norm.txt thrust_norm.txt lambda_norm.txt sequential_norm.txt stl_norm.txt omp_norm.txt
