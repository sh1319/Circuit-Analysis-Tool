#ifndef matrix_hpp
#define matrix_hpp

#include "circuit.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <complex>
#include <armadillo>
#include <utility>

using namespace std;
using namespace arma;

struct circuitmat{
	int numnode;
	fmat vmat; //(numnode - 1, 1)
	fmat imat; //(numnode - 1, 1)
	fmat gmat; //(numnode - 1, numnode -1)
	vector<float> simulation_time;
	fmat simulation_result; //(time.size(), numnode-1+1 (time column added))
};


circuitmat circtocircmat(circuit cir);
string find_source_voltage_node(circuit cir);
string find_source_current_node(circuit cir);
string source_type(circuit cir);
vector<string> remove_gnd_node(circuit cir);
vector<float> set_time(circuit cir);
fmat solve_matrix(circuitmat cmat, circuit cir, vector<string> list_of_node, float time);
circuit circuit_to_simplified_circuit_zero(circuit cir, circuitmat cmat); // same thing as below but at time zero
circuit circuit_to_simplified_circuit(circuit prevcir, circuitmat prevcmat, float time, float timestep); //change C -> DC V source, AC V -> DC V source, L -> DC I source, AC I -> DC I source
float current_betw_consec_nodes(component comp, circuit cir, circuitmat cmat, string nodeleft, string noderight);
float voltage_across_inductor(circuit cir, circuitmat cmat, string nodeleft, string noderight);

fmat find_current(circuit cir, circuitmat cmat);

#endif
