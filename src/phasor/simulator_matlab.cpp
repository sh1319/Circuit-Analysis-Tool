#include "matrix.hpp"

#include <iostream>
#include <armadillo>
#include <chrono>

using namespace std;
using namespace arma;
using namespace std::chrono;

int main(int argc, char **argv) {
    auto start = high_resolution_clock::now();
	// circuit netlist input to circuit data structure
	circuit cir = netlist_to_components();	
	// circuit data structure to circuit matrices						
	circuitmat circmat = circtocircmat(cir);			
	// finding admittance matrix			
	circmat.ymat = findymat(circmat, cir);		
	// simplifying admittance matrix given a voltage source					
	circmat.ymat = simplify_source_v(circmat, cir);				
	// setting values of current matrix	
	circmat.imat = set_imat(circmat, cir);					
	// solving for voltage matrix v = G^-1 * i		
	circmat.vmat = get_vmat(circmat, cir);							
	// finding time inputs for voltage phasor
	circmat.simulation_time = set_time(cir);						
	// removing gnd node from node lists
	vector<string> nodes = remove_gnd_node(circmat, cir);			
	// convert phasor to data by inputting time into continuous voltage function
	circmat.simulation_result = convert_phasor_to_data(circmat, cir);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop-start);
    cerr << "Program runtime: " << duration.count()* 0.001 << " s" << endl;
	// print simulation output
	circmat.simulation_result.print(cout);							
	cout << endl;
	return 0;
} 



