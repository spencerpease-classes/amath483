#!/bin/bash

trips=10

make clean
make hello_cuda_0.exe hello_cuda_1.exe hello_cuda_2.exe hello_cuda_3.exe sequential.exe omp.exe hello_thrust.exe

/bin/rm -f hello_cuda_1.txt
printf "=== hello_cuda_1 ===\n"
printf "N\ttrips\ttime\n" > hello_cuda_1.txt
for size in 18 19 20 21 ; do
    ./hello_cuda_1.exe ${size} ${trips} | tee -a hello_cuda_1.txt
done


for program in hello_cuda_2 hello_cuda_3 sequential omp hello_thrust ; do
    /bin/rm -f ${program}.txt
    printf "\n"
    printf "=== ${program} ===\n"
    printf "N\ttrips\ttime\n" > ${program}.txt
    for size in 20 21 22 23 24 25 26 27; do
	./${program}.exe ${size} ${trips} | tee -a ${program}.txt

    done
done

python plot.py hello_cuda_1.txt hello_cuda_2.txt  hello_cuda_3.txt sequential.txt omp.txt hello_thrust.txt
