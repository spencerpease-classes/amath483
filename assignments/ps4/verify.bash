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

do_test "make test_order.exe"
do_test "./test_order.exe"

do_test "make ordering.exe"
do_test "./ordering.exe"
if [ -e "ordering.png" ]; then
    echo "+++ ordering.png found +++"
else
    echo "--- ordering.png not found ---"
fi

do_test "./ordering.exe 32"
if [ -e "ordering.png" ]; then
    echo "+++ ordering.png found +++"
else
    echo "--- ordering.png not found ---"
fi

do_test "./ordering.exe 1024" 
if [ -e "ordering.png" ]; then
    echo "+++ ordering.png found +++"
else
    echo "--- ordering.png not found ---"
fi

do_test "make row_vs_col.exe"
do_test "./row_vs_col.exe"
if [ -e "row_vs_col.png" ]; then
    echo "+++ row_vs_col.png found +++"
else
    echo "--- row_vs_col.png not found ---"
fi

do_test "./row_vs_col.exe 32" 
if [ -e "row_vs_col.png" ]; then
    echo "+++ row_vs_col.png found +++"
else
    echo "--- row_vs_col.png not found ---"
fi

do_test "./row_vs_col.exe 1024" 
if [ -e "row_vs_col.png" ]; then
    echo "+++ row_vs_col.png found +++"
else
    echo "--- row_vs_col.png not found ---"
fi

do_test "make matvec.exe"
do_test "./matvec.exe"
if [ -e "matvec.png" ]; then
    echo "+++ matvec.png found +++"
else
    echo "--- matvec.png not found ---"
fi

do_test "make vov.exe"
do_test "./vov.exe"
if [ -e "vov.png" ]; then
    echo "+++ vov.png found +++"
else
    echo "--- vov.png not found ---"
fi
