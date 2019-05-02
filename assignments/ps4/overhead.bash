#!/bin/bash

echo "================================================================"
echo "No Optimization"
echo ""
make clean > /dev/null 2>&1
make overhead.exe OPTS="-O0" > /dev/null 2>&1
echo "Skipped "
# ./overhead.exe
echo "================================================================"
echo ""
echo ""
echo ""

echo "================================================================"
echo "With O3 optimization"
echo ""
make clean > /dev/null 2>&1
make overhead.exe OPTS="-O3 -DNDEBUG" > /dev/null 2>&1
./overhead.exe
echo "================================================================"
echo ""
echo ""
echo ""
