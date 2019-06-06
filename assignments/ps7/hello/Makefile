#
# This file is part of the course materials for AMATH 483/583 at the University of Washington,
# Spring 2019
#
# Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
# https://creativecommons.org/licenses/by-nc-sa/4.0/
#
# Author: Andrew Lumsdaine
# 


# Choose features here with yes or no
OMP	:= yes
THREADS	:= no
PYTHON  := no
CIMG    := no


# These must be set to some value or make will pick defaults (which will be wrong)
CXX	:= c++
LANG	:= -std=c++11

TARGETS := hello_cuda_0.exe hello_cuda_1.exe hello_cuda_2.exe hello_cuda_3.exe sequential.exe omp.exe hello_thrust.exe

.PHONY : all

include ../include/Makefile.inc

all    : $(TARGETS)

OTHERCLEAN = $(TARGETS:.exe=.txt)


# The following 4 (yes 4) lines must be in all the subdirectory makefiles
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE

# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
