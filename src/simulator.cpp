#include "matrix.hpp"
#include <iostream>
#include <armadillo>
#include <chrono>

using namespace std;
using namespace arma;
using namespace std::chrono;

/*
assumptions:
the main AC source is connected to a GND node and another node
only circuits with 1 main AC source, a number of R's, L's, C's, DC V sources, DC I sources will work on this simulation

improvements:
node list organized at random order
adding labels to matrices for readability
orgarising nodes list in some sort of order
*/

int main(int argc, char **argv) {
    auto start = high_resolution_clock::now();
    // initial, current, previous circuits and circuit matrices + variables for timing of simulation + matrix for the result of simulation (data) + timestep to count the progress
    circuit initial_cir = netlist_to_circuit(); // netlist to circuit data structure
    circuitmat initial_cmat = circtocircmat(initial_cir);
    circuit current_circuit = initial_cir;
    circuitmat current_cmat = initial_cmat;
    circuit previous_circuit;
    circuitmat previous_cmat;
    vector<float> time = set_time(initial_cir);
    float current_time = initial_cir.simulation.start;
    // assuming that node list does not change -> especially when considering R, L, C circuits
    vector<string> node_list = remove_gnd_node(initial_cir);
    fmat simulation_data, current_data;
    simulation_data.zeros(time.size(), node_list.size()+1+initial_cir.components_list.size());
    current_data.zeros(time.size(), initial_cir.components_list.size());
    int current_timestep = 0;
    float limit_timestep = (initial_cir.simulation.stop - initial_cir.simulation.start) / initial_cir.simulation.max_step;
    
    
    // SIMULATION
    while(current_time <= initial_cir.simulation.stop) {
        //cerr << current_time << endl;
        if(current_timestep == 0) // at time = 0
        { 
            simulation_data.at(0,0) = current_time;
            current_circuit = circuit_to_simplified_circuit_zero(initial_cir, initial_cmat); // at time zero replace all C and L to V and I sources
            current_cmat = circtocircmat(current_circuit);
            current_cmat.vmat = solve_matrix(current_cmat, current_circuit, node_list, current_time); // vmat dimension = [node_list.size() x 1]
            for(unsigned k=0; k<node_list.size(); ++k) {
                simulation_data.at(0,k+1) = current_cmat.vmat(k,0); // should always be zero given that there is no dc offset in the main AC source
            }
        }
        else // at time > 0 && time <= simulaton stop time
        {   
            current_circuit = circuit_to_simplified_circuit(previous_circuit, previous_cmat, current_time, initial_cir.simulation.max_step);
            current_cmat = circtocircmat(current_circuit);
            current_cmat.vmat = solve_matrix(current_cmat, current_circuit, node_list, current_time); // vmat dimension = [node_list.size() x 1]
            current_data = find_current(current_circuit, current_cmat);
            for(unsigned k=0; k<node_list.size()+initial_cir.components_list.size(); ++k) {
                if(k<node_list.size()) {
                    simulation_data.at(current_timestep,k+1) = current_cmat.vmat(k,0);
                }
                else {
                    simulation_data.at(current_timestep,k+1) = current_data(k-node_list.size(),0);
                }
            }
            if(current_timestep<=limit_timestep) {
                simulation_data.at(current_timestep,0) = current_time;
            }
        }
        current_time += initial_cir.simulation.max_step;
        current_timestep++;
        previous_circuit = current_circuit;
        previous_cmat = current_cmat;
        //cerr << endl << endl;
    }
        auto stop = high_resolution_clock::now();

    // Data out in suitable form [TIME NODE1 NODE2 NODE3 ... NODEn]
	cerr << "      " << "TIME ";
	for(unsigned i=0; i<node_list.size(); i++) {
		cerr << "        " << node_list[i];
	}
    for(unsigned i=0; i<initial_cir.components_list.size(); i++) {
		cerr << "           " << initial_cir.components_list[i].name;
	}
	cerr << endl;
    simulation_data.print(cout);
    auto duration = duration_cast<microseconds>(stop-start);
    cerr << "Program runtime: " << duration.count()* 0.000001 << " s" << endl;
    return 0;
}