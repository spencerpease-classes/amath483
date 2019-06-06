#
# This file is part of the course materials for AMATH483/583 at the University of Washington,
# Spring 2019
#
# Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
# https://creativecommons.org/licenses/by-nc-sa/4.0/
#
# Author: Andrew Lumsdaine
# 

ifndef CXX
CXX		:= c++
endif

ifndef NVCC
NVCC		:= /usr/local/cuda/bin/nvcc
endif

OS		:= $(shell uname -s)

ifeq ($(CIMG),yes)
	DEFS		+= -D__USE_CIMG
	XINCLUDES	+= -I/opt/X11/include
	XLIBS		+= -L/opt/X11/lib -lX11
endif


ifeq ($(PYTHON),yes)
	PYINCLUDES	:= -I/usr/include/python2.7
	ifeq ($(OS),Linux)
		PYLIBS  := -L/usr/lib/python2.7/config-x86_64-linux-gnu 
	endif
	PYLIBS		+= -lpython2.7 
endif


ifeq ($(MODE),debug)
	OPTS	:= -g
	CUOPTS  := -g
else
	OPTS	:= -Ofast -march=native -DNDEBUG 
	CUOPTS  := -O3 -DNDEBUG
endif


ifeq ($(OMP),yes)
	ifeq ($(OS),Darwin)
		DEFS += -Xpreprocessor
	endif
	DEFS += -fopenmp
	LIBS += -lomp
endif

ifeq ($(THREADS),yes)
	ifeq ($(OS),Linux)
		LIBS	+= -lpthread
	endif
endif

ifndef LANG
LANG		:= -std=c++11
endif

PICKY		:= -Wall
DEFS		+= 
REPORTING   	= -Rpass=.*
INCLUDES	+= -I../include $(XINCLUDES) $(PYINCLUDES)
VPATH		+= ../include
LIBS		+= $(XLIBS) $(PYLIBS)

CXXFLAGS	+= $(DEFS) $(OPTS) $(LANG) $(PICKY) $(INCLUDES)
NVCCFLAGS	+= $(CUOPTS) $(LANG) $(INCLUDES) -I/usr/local/cuda/include --expt-extended-lambda -ccbin c++ 

AMATH583	+= Matrix.hpp Vector.hpp CSRMatrix.hpp amath583.hpp amath583sparse.hpp amath583IO.hpp Timer.hpp 

HEADERS		:= $(AMATH583)
SOURCES		:= $(TARGETS:.exe=.cpp)
OBJECTS		:= $(SOURCES:.cpp=.o)
PCHS		:= $(HEADERS:=.gch)

.PHONY		: defreport optreport clean depend all

all		: $(TARGETS)

%.exe       : %.o
		      $(NVCC) $^ -o $@ $(LIBS)

%.o 		: %.cpp
		      $(CXX) -c $(CXXFLAGS) $< -o $@

%.o     	: %.cu
			  $(NVCC) -c $(NVCCFLAGS) $< -o $@

%.s 		: %.cpp
		      $(CXX) -S $(CXXFLAGS) $<

defreport	:
		  $(CXX) -dM -E -x c++ /dev/null

optreport	:
		  echo 'int;' | $(CXX) -xc++ $(CXXFLAGS) - -o /dev/null -\#\#\#

clean		:
		  /bin/rm -f $(TARGETS) $(OBJECTS) $(PCHS) Matrix.s a.out *~ Makefile.bak $(OTHERCLEAN)

depend		: 
	@ $(CXX) -MM $(LANG) $(INCLUDES) $(SOURCES) > makedep
	@ echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	@ echo '$$r makedep' >>eddep
	@ echo 'w' >>eddep
	@ cp Makefile Makefile.bak
	@ ed - Makefile < eddep
	@ /bin/rm eddep makedep
	@ echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	@ echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	@ echo '# see make depend above' >> Makefile

# The following 4 (yes 4) lines must be in all the subdirectory makefiles
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE

# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
