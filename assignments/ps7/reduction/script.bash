#!/bin/bash

make clean 
make all

for kernel in 0 1 2 3 4 5 6; do

    echo "================================================================"
    echo "Kernel ${kernel}"
    
nvprof --metrics achieved_occupancy,branch_efficiency,sysmem_read_throughput,dram_read_throughput,gld_requested_throughput,gst_throughput,gst_efficiency,sm_efficiency,shared_efficiency,gld_effiency --events branch,divergent_branch --print-gpu-summary ./reduction --kernel=${kernel} |& tail -24

done



