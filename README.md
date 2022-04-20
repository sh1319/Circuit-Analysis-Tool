# C++ Circuit Simulator

# Introduction
SPICE input netlist is parsed to the compiled C++ simulator program to perform the transient analysis and to output the simulation result data to be plotted using MATLAB code.
```

       +---------+   +-----------+   +-------------+   
       |  SPICE  |   |    C++    |   | Output Data |   
       |  Input  |-->| Simulator |-->|  Plotting   |
       | Netlist |   |  Program  |   | with MATLAB |
       +---------+   +-----------+   +-------------+

```

# Overview of Scripts

## Header File
All source files come under the header files: circuit.hpp and matrix.hpp where matrix.hpp comes under circuit.hpp file.
```
                    +-------------+
                    | circuit.hpp |
                    |      |      |
                    |  matrix.hpp |
                    |      |      |
                    |     *.cpp   |
                    +-------------+
```
circuit.hpp -> Contains the data structure to model circuits, its components and the simulation 
<br>
matrix.hpp -> Contains the data structure for matrix representation of circuits to be used for matrix calculations to output the result

## Source File
circuit.cpp: Contains fundamental function for conversion of netlist to circuit data structure
<br>
matrix.cpp: Contains fundamental function for conversion of circuits to matrix representations
<br>
matrix_update.cpp: Contains function that updates the circuit every timestep
<br>
matrix_solver.cpp: Contains solve_matrix function that finds the matrix representative of the given circuit and solves for all node voltages
<br>
matrix_current.cpp: Contains functions to find the current through different components
<br>
simulator.cpp: Contains the main function for simulation with all circuit updates and matrix computations for each timestep

# Required C++ Library
## Armadillo
Armadillo is a C++ Linear Algebra Library. <br>
The documentation can be found in the link below. <br>
http://arma.sourceforge.net/docs.html
```c++
#include <armadillo>
using namespace arma;
```

# Simulator
## Build the Simulator
```bash
make 
```
## Run Simulation & Pipeline Output
```bash
./simulator <netlist.txt >output.txt 
```
## Clean up Object Files
```bash
make clean
```

# Plotting Output with MATLAB
Simulator outputs the time taken for simulation and the labels (voltage/current of the nodes/components) for each columns of data that was pipelined into output text file. This file can be loaded with MATLAB following the formats used by MATLAB files in the folder simulation_plot.