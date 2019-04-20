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

do_test "make timing0.exe timing3.exe efficiency0.exe efficiency3.exe"
do_test "make float_vs_double0.exe float_vs_double3.exe"
do_test "make plus_equals.exe star_times.exe"






    
