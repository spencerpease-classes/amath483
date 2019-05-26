#!/bin/bash


function do_test() {
    echo "=== Testing $1 ==="
    $1
    if [ $? == 0 ] ; then
	echo "+++ $1 Passed +++"
    else
	echo "--- $1 Failed ---"
    fi
}


do_test "make clean"

do_test "make norm.exe"
do_test "./norm.exe"

do_test "make pnorm.exe"
do_test "./pnorm.exe"
if [ -e "partitioned_vs_recursive.png" ]; then
    echo "+++ partitioned_vs_recursive.png found +++"
else
    echo "--- partitioned_vs_recursive.png not found ---"
fi

do_test "make matvec.exe"
do_test "./matvec.exe"
if [ -e "par_matvec.png" ]; then
    echo "+++ par_matvec.png found +++"
else
    echo "--- par_matvec.png not found ---"
fi
