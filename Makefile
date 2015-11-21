CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD -gdwarf-3
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

SOURCES = ${wildcard *.cc} 			# build all ".cc" files
OBJECTS = ${SOURCES:.cc=.o}			# substitute ".cc" with ".o"
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXEC = soda

#############################################################

.PHONY : all clean

all : ${EXEC}					# build all executables

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC} ImplType
