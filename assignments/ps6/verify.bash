#!/bin/bash


function do_test() {
    echo "=== Testing $* ==="
    $*
    if [ $? == 0 ] ; then
	echo "+++ $* Passed +++"
    else
	echo "--- $* Failed ---"
    fi
}


do_test make clean

do_test make norm_scale.exe

do_test ./norm_scale.exe -N 5000
mv norm_scale.png norm_scale_small.png 

do_test ./norm_scale.exe -N 50000000
mv norm_scale.png norm_scale_large.png 

do_test ./norm_scale.exe -N 500000 -s strong
mv norm_scale.png norm_scale_strong.png 

do_test ./norm_scale.exe -N 500000 -s weak
mv norm_scale.png norm_scale_weak.png 



do_test make clean

do_test make matvec_scale.exe

do_test ./matvec_scale.exe -N 1000
mv matvec_scale.png matvec_scale_small.png 

do_test ./matvec_scale.exe -N 10000000
mv matvec_scale.png matvec_scale_large.png 

do_test ./matvec_scale.exe -N 100000 -s strong
mv matvec_scale.png matvec_scale_strong.png 

do_test ./matvec_scale.exe -N 100000 -s weak
mv matvec_scale.png matvec_scale_weak.png 



do_test make clean

do_test make csr_omp.exe

do_test ./csr_omp.exe -i cit-Patents.mmio


do_test make clean

do_test make csr_scale.exe

do_test ./csr_scale.exe -i cit-Patents.mmio


do_test make clean

do_test make pagerank.exe

do_test pagerank.exe -i cit-Patents.mmio
