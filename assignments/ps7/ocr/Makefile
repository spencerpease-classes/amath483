#
# This file is part of the course materials for CSE P 524 at the University of Washington,
# Autumn 2018
#
# Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
# https://creativecommons.org/licenses/by-nc-sa/4.0/
#
# Author: Andrew Lumsdaine
# 

# Choose features here with yes or no
# Choosing yes for CIMG will display the interactive GUI, but requires an X server
OMP	:= no
THREADS	:= no
PYTHON  := no
CIMG    := no


# Set to debug to turn debugging on
MODE	:= 


# These must be set to some value or make will pick defaults (which will be wrong)
NVCC 	:= c++
CXX	:= c++
LANG	:= -std=c++14

TARGETS := ocr.exe

.PHONY : all

ocr.exe: ocr.o

include ../include/Makefile.inc

all    : $(TARGETS)


# The following 4 (yes 4) lines must be in all the subdirectory makefiles
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
ocr.o: ocr.cpp ../include/Matrix.hpp ../include/Vector.hpp \
  ../include/Timer.hpp ../include/amath583.hpp
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
