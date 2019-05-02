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

# No optimization, plus debugging symbols
# OPTS            := -g -O0

# Maximum optimization effort: -O3 -march=native -DNDEBUG
OPTS            := -O3 -march=native
LANG            := -std=c++11
PICKY           := -Wall
DEFS		:= -D__USE_CIMG
INCLUDES	:= -I../include 
LIBS		:= -lpython2.7

XINCLUDES	:= -I/opt/X11/include -I/usr/include/python2.7
XLIBS		:= -L/opt/X11/lib -lX11  
VPATH		= ../src:../test

INCLUDES	+= $(XINCLUDES)
LIBS		+= $(XLIBS)

OS		:= $(shell uname -s)
ifeq ($(OS),Linux)
	LIBS	+= -lpthread
endif


CXXFLAGS	+= $(DEFS) $(OPTS) $(LANG) $(PICKY) $(INCLUDES) 


TARGETS		:= overhead.exe ordering.exe test_order.exe row_vs_col.exe vov.exe matvec.exe
SOURCES		:= $(TARGETS:.exe=.cpp)
HEADERS		:= $(SOURCES:.cpp=.hpp)
OBJECTS		:= $(SOURCES:.cpp=.o) amath583.o
PCHS		:= $(HEADERS:=.gch)


.PHONY		: defreport optreport clean depend all

all		: $(TARGETS)

%.exe        	: %.o
		  $(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

%.o 		: %.cpp
		  $(CXX) -c $(CXXFLAGS) $< -o $@

%.s 		: %.cpp
		  $(CXX) -S $(CXXFLAGS) $<

overhead.exe	: overhead.o   amath583.o
ordering.exe	: ordering.o   amath583.o
test_order.exe  : test_order.o amath583.o
matvec.exe	: matvec.o     amath583.o
vov.exe		: vov.o        amath583.o

defreport	:
		  $(CXX) -dM -E -x c++ /dev/null

optreport	:
		  echo 'int;' | $(CXX) -xc++ $(CXXFLAGS) - -o /dev/null -\#\#\#

clean		:
		  /bin/rm -f $(TARGETS) $(OBJECTS) $(PCHS) Matrix.s a.out *~ Makefile.bak

depend		: 
	@ $(CXX) -MM $(LANG) $(INCLUDES) $(SOURCES) $(TESTS) > makedep
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
overhead.o: overhead.cpp Timer.hpp Matrix.hpp amath583.hpp Vector.hpp
ordering.o: ordering.cpp Matrix.hpp Timer.hpp amath583.hpp Vector.hpp 
test_order.o: test_order.cpp catch.hpp Matrix.hpp amath583.hpp Vector.hpp
row_vs_col.o: row_vs_col.cpp RowMatrix.hpp ColMatrix.hpp Timer.hpp amath583_rvc.hpp matplotlibcpp.h 
vov.o: vov.cpp VoVMatrix.hpp Matrix.hpp amath583.hpp amath583_vov.hpp matplotlibcpp.h
matvec.o: matvec.cpp Vector.hpp Matrix.hpp amath583.hpp matplotlibcpp.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
