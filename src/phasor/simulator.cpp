//SIMULATOR
/*
x include GND node as nodes number count
V as phasor to calculate each node first then convert to continuous v sine function
*/
#include "matrix.hpp"

#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;


int main(int argc, char **argv) {
	circuit cir = netlist_to_components();
	print_circuit_values(cir);
	circuitmat circmat = circtocircmat(cir);
	/*
	circuitmat circmat;
	circmat.numnode = cir.node_list.size();
	circmat.gmat.zeros(circmat.numnode-1,circmat.numnode-1);
	*/
	//component comp = find_component(cir, "VIN", "VOUT");
	//cout << cir.node_list.size() << endl;
	circmat.gmat = findgmat(circmat, cir);

	cout << "Conductance Matrix After Manipulation: " << endl;
	circmat.gmat.print(cout);
	cout << endl;

	cout << "Simplify Given Source Voltage: " << endl;
	circmat.gmat = simplify_source_v(circmat, cir);
	circmat.gmat.print(cout);
	cout << endl;

	cout << "Current Matrix: " << endl;
	circmat.imat = set_imat(circmat, cir);
	circmat.imat.print(cout);
	cout << endl;
	
	circmat.vmat = get_vmat(circmat, cir);
	cout << "Voltage Matrix: " << endl;
	circmat.vmat.print(cout);
	cout << endl;
	
	cout << "Simulation Time Steps: " << endl;
	circmat.simulation_time = set_time(cir);
	for(unsigned i=0; i<circmat.simulation_time.size(); ++i) {
		cout << circmat.simulation_time[i] << " ";
	}
	cout << endl << endl;

	vector<string> nodes = remove_gnd_node(circmat, cir);
	cout << "Simulation Data per Time Steps: " << endl;
	cout << "      " << "TIME";
	for(unsigned i=0; i<nodes.size(); i++) {
		cout << "          " << nodes[i];
	}
	cout << endl;
	circmat.simulation_result = convert_phasor_to_data(circmat, cir);
	circmat.simulation_result.print(cout);
	cout << endl;
	return 0;
} 


