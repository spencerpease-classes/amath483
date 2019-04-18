#
# This file is part of the course materials for AMATH483/583 at the University of Washington,
# Spring 2019
#
# Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
# https://creativecommons.org/licenses/by-nc-sa/4.0/
#
# Author: Andrew Lumsdaine
#

CXX             := c++

OPTS            := -g -O0
LANG            := -std=c++11
PICKY           := -Wall

CXXFLAGS        := $(LANG) $(OPTS) $(PICKY)

.PHONY		: clean all

all		: julia.exe test_norms.exe test_zeroize.exe test_dot.exe \
		  assert0.exe assert1.exe assert2.exe assert3.exe

julia.exe 	: julia.o amath583.o
		  $(CXX) $(CXXFLAGS) -lpthread julia.o -o julia.exe

julia.o 	: julia.cpp amath583.hpp Vector.hpp
	   	  $(CXX) $(CXXFLAGS) -c julia.cpp -o julia.o

test_norms.exe 	: test_norms.o amath583.o
		  $(CXX) $(CXXFLAGS) test_norms.o amath583.o -o test_norms.exe

test_norms.o 	: test_norms.cpp amath583.hpp Vector.hpp
		  $(CXX) $(CXXFLAGS) -c test_norms.cpp -o test_norms.o

test_zeroize.exe 	: test_zeroize.o amath583.o
		  $(CXX) $(CXXFLAGS) test_zeroize.o amath583.o -o test_zeroize.exe

test_zeroize.o 	: test_zeroize.cpp amath583.hpp Vector.hpp
		  $(CXX) $(CXXFLAGS) -c test_zeroize.cpp -o test_zeroize.o

test_dot.exe 	: # write me
		  # write me

test_dot.o 	: # write me
		  # write me

amath583.o 	: amath583.cpp amath583.hpp Vector.hpp
		  $(CXX) $(CXXFLAGS) -c amath583.cpp -o amath583.o

assert0.exe	: assert0.cpp
		  $(CXX) $(CXXFLAGS) assert0.cpp -o assert0.exe

assert1.exe	: assert1.cpp
		  $(CXX) $(CXXFLAGS) assert1.cpp -o assert1.exe

assert2.exe	: assert2.cpp
		  $(CXX) $(CXXFLAGS) assert2.cpp -o assert2.exe

assert3.exe	: assert3.cpp
		  $(CXX) $(CXXFLAGS) -DNDEBUG assert3.cpp -o assert3.exe

clean		:
		  /bin/rm -f julia.o amath583.o julia.exe *~ julia.bmp \
		  assert0.exe assert1.exe assert2.exe assert3.exe \
		  test_norms.o test_norms.exe test_zeroize.o test_zeroize.exe \
		  test_dot.o test_dot.exe
		  /bin/rm -rf assert0.exe.dSYM assert1.exe.dSYM assert2.exe.dSYM assert3.exe.dSYM
