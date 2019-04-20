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

all		: hello.exe goodbye.exe

LIBS		:=
OS		:= $(shell uname -s)
ifeq ($(OS),Linux)
	LIBS	+= -lpthread
endif

hello.exe	: hello.cpp
		  $(CXX) $(CXXFLAGS) hello.cpp -o hello.exe

goodbye.exe	: goodbye.cpp
		  $(CXX) $(CXXFLAGS) goodbye.cpp -o goodbye.exe

repeat.exe	: repeat.cpp
		  $(CXX) $(CXXFLAGS) repeat.cpp -o repeat.exe

main.exe	: main.cpp
		  $(CXX) $(CXXFLAGS) main.cpp -o main.exe

timing0.exe	: timing.cpp
		  $(CXX) $(LANG) $(PICKY) -O0 timing.cpp -o timing0.exe

timing3.exe	: timing.cpp
		  $(CXX) $(LANG) $(PICKY) -O3 timing.cpp -o timing3.exe

efficiency0.exe	: efficiency.cpp
		  $(CXX) $(LANG) $(PICKY) -O0 efficiency.cpp -o efficiency0.exe

efficiency3.exe	: efficiency.cpp
		  $(CXX) $(LANG) $(PICKY) -O3 efficiency.cpp -o efficiency3.exe

float_vs_double0.exe 	: float_vs_double.cpp
		  $(CXX) $(LANG) $(PICKY) -O0 float_vs_double.cpp -o float_vs_double0.exe

float_vs_double3.exe 	: float_vs_double.cpp
		  $(CXX) $(LANG) $(PICKY) -O3 float_vs_double.cpp -o float_vs_double3.exe

plus_equals.exe 	: plus_equals.o amath583.o
		  $(CXX) $(CXXFLAGS) plus_equals.o amath583.o -o plus_equals.exe

plus_equals.o 	: plus_equals.cpp amath583.hpp Vector.hpp
		  $(CXX) $(CXXFLAGS) -c plus_equals.cpp -o plus_equals.o

star_times.exe 	: star_times.o amath583.o
		  $(CXX) $(CXXFLAGS) star_times.o amath583.o -o star_times.exe

star_times.o 	: star_times.cpp amath583.hpp Vector.hpp
		  $(CXX) $(CXXFLAGS) -c star_times.cpp -o star_times.o

amath583.o 	: amath583.cpp amath583.hpp Vector.hpp
		  $(CXX) $(CXXFLAGS) -c amath583.cpp -o amath583.o

clean		:
		  /bin/rm -f amath583.o \
		  hello.exe goodbye.exe repeat.exe main.exe \
		  timing0.exe timing3.exe efficiency0.exe efficiency3.exe \
		  float_vs_double0.exe float_vs_double3.exe \
		  plus_equals.o plus_equals.exe star_times.o star_times.exe
		  /bin/rm -rf *.exe.dSYM
