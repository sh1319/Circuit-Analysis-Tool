#ifndef matrix_hpp
#define matrix_hpp

#include "phasor.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <complex>
#include <armadillo>

using namespace std;
using namespace arma;

struct circuitmat{
	int numnode;
	cx_fmat vmat; //(numnode - 1, 1)
	cx_fmat imat; //(numnode - 1, 1)
	cx_fmat gmat; //(numnode - 1, numnode -1)
	vector<float> simulation_time;
	fmat simulation_result; //(time.size(), numnode-1+1 (time column added))
};


circuitmat circtocircmat(circuit cir);
cx_fmat findgmat(circuitmat cmat, circuit cir);
cx_fmat simplify_source_v(circuitmat cmat, circuit cir);
vector<string> remove_gnd_node(circuitmat cmat, circuit cir);
cx_fmat set_imat(circuitmat cmat, circuit cir);
cx_fmat get_vmat(circuitmat cmat, circuit cir);
vector<float> set_time(circuit cir);
fmat convert_phasor_to_data(circuitmat cmat, circuit cir);

#endif
