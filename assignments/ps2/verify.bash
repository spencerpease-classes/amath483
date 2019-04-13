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

do_test "make assert3.exe"
do_test "./assert3.exe"

do_test "make test_norms.exe"
do_test "./test_norms.exe"

do_test "make test_zeroize.exe"
do_test "./test_zeroize.exe"

do_test "make julia.exe"
do_test "./julia.exe"
if [ -e "julia.bmp" ]; then
    echo "+++ julia.bmp found +++"
else
    echo "--- julia.bmp not found ---"
fi




    
