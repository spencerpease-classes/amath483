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

# Uncomment this to test CSC
CSC		+= -DTEST_CSC

# Uncomment this to time CSC
CSC		+= -DTIME_CSC

# Uncomment this to test AOS
# AOS		+= -DTEST_AOS

# Uncomment this to time AOS
# AOS		+= -DTIME_AOS

# Maximum optimization effort: -O3 -march=native -DNDEBUG
OPTS            := -O3 -march=native -DNDEBUG
LANG            := -std=c++17
PICKY           := -Wall
DEFS		:= $(CSC) $(AOS)
INCLUDES	:= -I/usr/include/python2.7
LIBS		:= -lpython2.7

INCLUDES	+= $(XINCLUDES)
LIBS		+= $(XLIBS)

OS		:= $(shell uname -s)
ifeq ($(OS),Linux)
	LIBS	+= -lpthread
endif

CXXFLAGS	+= $(DEFS) $(OPTS) $(LANG) $(PICKY) $(INCLUDES)

TARGETS		:= matvec_test.exe matmat_test.exe matvec_time.exe matmat_time.exe
SOURCES		:= $(TARGETS:.exe=.cpp)
HEADERS		:= $(SOURCES:.cpp=.hpp)
OBJECTS		:= $(SOURCES:.cpp=.o) amath583.o amath583IO.o amath583sparse.o
PCHS		:= $(HEADERS:=.gch)

.PHONY		: defreport optreport clean depend all

all		: $(TARGETS)

%.exe        	: %.o
		  $(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

%.o 		: %.cpp
		  $(CXX) -c $(CXXFLAGS) $< -o $@

%.s 		: %.cpp
		  $(CXX) -S $(CXXFLAGS) $<

matvec_test.exe	: matvec_test.o amath583.o amath583sparse.o amath583IO.o
matvec_time.exe	: matvec_time.o amath583.o amath583sparse.o amath583IO.o
matmat_test.exe	: matmat_test.o amath583.o amath583sparse.o amath583IO.o
matmat_time.exe	: matmat_time.o amath583.o amath583sparse.o amath583IO.o

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
matvec_test.o: matvec_test.cpp catch.hpp Vector.hpp COOMatrix.hpp \
  Matrix.hpp CSRMatrix.hpp amath583.hpp amath583IO.hpp CSCMatrix.hpp \
  AOSMatrix.hpp amath583sparse.hpp
matmat_test.o: matmat_test.cpp catch.hpp Vector.hpp COOMatrix.hpp \
  Matrix.hpp CSRMatrix.hpp amath583.hpp amath583IO.hpp CSCMatrix.hpp \
  AOSMatrix.hpp amath583sparse.hpp
matvec_time.o: matvec_time.cpp Timer.hpp amath583.hpp Matrix.hpp \
  Vector.hpp amath583IO.hpp COOMatrix.hpp CSRMatrix.hpp CSCMatrix.hpp \
  AOSMatrix.hpp amath583sparse.hpp matplotlibcpp.h
matmat_time.o: matmat_time.cpp Matrix.hpp Timer.hpp amath583.hpp \
  Vector.hpp amath583IO.hpp COOMatrix.hpp CSRMatrix.hpp CSCMatrix.hpp \
  AOSMatrix.hpp amath583sparse.hpp matplotlibcpp.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
