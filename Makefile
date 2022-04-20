CC = g++
LINKERFLAG = -O2 -larmadillo
OBJS = matrix.o \
	   matrix_update.o \
	   matrix_solver.o \
	   matrix_current.o \
	   circuit.o \
	   simulator.o

OBJS_SRC = src/matrix.o \
	   	   src/matrix_update.o \
	       src/matrix_solver.o \
	   	   src/matrix_current.o \
	   	   src/circuit.o \
	   	   src/simulator.o

all: simulator

simulator: ${OBJS_SRC}
	${CC} -o simulator ${OBJS} ${LINKERFLAG}

${OBJS}: src/matrix.hpp

.cpp.o:
	${CC} -c $<

clean:
	echo cleaning up; /bin/rm -f core *.o