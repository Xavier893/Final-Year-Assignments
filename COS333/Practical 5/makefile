# Makefile for compiling Fortran and COBOL programs

# Compiler and flags for Fortran
FC = gfortran
FFLAGS = -o s20526254

# Compiler and flags for COBOL
COBC = cobc
COBFLAGS = -x -fixed -o SimpleStats

# Default target to compile both programs
all: fortran cobol

# Target to compile Fortran program
fortran:
	$(FC) $(FFLAGS) s20526254.f

# Target to compile COBOL program
cobol:
	$(COBC) $(COBFLAGS) s20526254.cob

run-fortran:
	./s20526254

run-cobol:
	./SimpleStats

# Clean target to remove compiled files
clean:
	rm -f s20526254 SimpleStats
