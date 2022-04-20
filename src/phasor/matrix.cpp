#include "matrix.hpp"

using namespace std;
using namespace arma;

circuitmat circtocircmat(circuit cir){
	circuitmat circmat;
	circmat.numnode = cir.node_list.size();
    circmat.gmat.zeros(circmat.numnode-1,circmat.numnode-1);
    circmat.imat.zeros(circmat.numnode-1,1);
    circmat.vmat.zeros(circmat.numnode-1,1);
    return circmat;
}

vector<string> remove_gnd_node(circuitmat cmat, circuit cir) {
    vector<string> nodes;
    for(unsigned j=0; j<cir.node_list.size(); j++) {
        if(cir.node_list[j] != "0") {
            nodes.push_back(cir.node_list[j]);
        }
    }
    return nodes;
}

cx_fmat findgmat(circuitmat cmat, circuit cir){
    vector<string> nodes = remove_gnd_node(cmat, cir); //MAPPING NEEDS TO BE DONE
    vector<component> components_betw_ij;
//    cout << "Conductance Matrix All Initialised to Zeros: " << endl;
//    cmat.gmat.print(cout);
//    cout << endl;

    //ASSUME G11 here G00 is the node connected to V1
	for(int i = 0; i<cmat.numnode - 1; i++) {
		for(int j = 0; j<cmat.numnode - 1; j++) {
			if(i != j) {
                components_betw_ij = find_component(cir, nodes[i], nodes[j]);
                for(unsigned k=0; k<components_betw_ij.size(); ++k) {
				    cmat.gmat(i,j) += find_conductance(components_betw_ij[k], cir.voltages_src.frequency);
                }
                cmat.gmat(i,j) = -cmat.gmat(i,j);
            }
            else {
                cmat.gmat(i,j) = find_node_conductance(cir, nodes[i]);
            }
		}
	}
    return cmat.gmat;
}

cx_fmat simplify_source_v(circuitmat cmat, circuit cir) {
    for(int j=0; j<cmat.numnode - 1; j++) {
        if(cir.node_list[j] == "Vin" || cir.node_list[j] == "VIN") {
            cmat.gmat.at(j,j) = {1,0};
            cmat.gmat.at(j,j+1) = {0,0};
        }
    }
    return cmat.gmat;
}

cx_fmat set_imat(circuitmat cmat, circuit cir) {
    for(int j=0; j<cmat.numnode - 1; j++) {
        if(cir.node_list[j] == "Vin" || cir.node_list[j] == "VIN") {
            cmat.imat.at(j,0) = sine_to_phasor(cir.voltages_src.amplitude, cir.voltages_src.dc_offset);
        }
    }

    return cmat.imat;
}

cx_fmat get_vmat(circuitmat cmat, circuit cir) {
    cx_fmat inv_gmat = inv(cmat.gmat);
//    cout << "Inverse of Conductance Matrix: " << endl;
//    inv_gmat.print(cout);
//    cout << endl;
    cmat.vmat = inv_gmat * cmat.imat;
    return cmat.vmat;
}

vector<float> set_time(circuit cir) {
    vector<float> time;
    float t = cir.simulation.start;
    while(t <= cir.simulation.stop) {
        if(t >= cir.simulation.save_data) {
            time.push_back(t);
        }
        t += cir.simulation.max_step;
    }
    return time;
}

fmat convert_phasor_to_data(circuitmat cmat, circuit cir) {
    fmat data;
    vector<string> nodes = remove_gnd_node(cmat, cir);
    int order = number_of_l_and_c(cir);
    data.zeros(cmat.simulation_time.size(), cmat.numnode);
    for(unsigned i=0; i<cmat.simulation_time.size(); ++i) {
        data.at(i,0) = cmat.simulation_time[i];
        for(unsigned j=1; j<cmat.numnode; ++j) {
            if(nodes[j-1] == "VIN" || nodes[j-1] == "Vin") {
                data.at(i,j) = phasor_at_t_src_v(cmat.vmat.at(j-1,0), cir.voltages_src.frequency, cmat.simulation_time[i], cir.voltages_src.dc_offset);
            }
            else {
                if(order == 0 || order == 1) {
                    data.at(i,j) = phasor_at_t_other_nodes(cmat.vmat.at(j-1,0), cir.voltages_src.frequency, cmat.simulation_time[i]) + first_order_transient(cmat.vmat.at(j-1,0), cmat.simulation_time[i], get_time_constant(cir));
                }
                else if(order == 2) {
                    float R, L, C;
                    for(unsigned k=0; k<cir.components_list.size(); ++k) {
                        if(component_type(cir.components_list[k].name) == 'R') {
                            R = cir.components_list[k].floatvalue;
                        }
                        else if(component_type(cir.components_list[k].name) == 'L') {
                            L = cir.components_list[k].floatvalue;
                        }
                        else if(component_type(cir.components_list[k].name) == 'C') {
                            C = cir.components_list[k].floatvalue;
                        }
                        else {;}
                    }
                    data.at(i,j) = phasor_at_t_other_nodes(cmat.vmat.at(j-1,0), cir.voltages_src.frequency, cmat.simulation_time[i]) + second_order_transient(cmat.vmat.at(j-1,0), cmat.simulation_time[i], R, L, C);
                }
                else if(order == 3) {
                    ; //TODO
                }
                else {assert(0);}
            }
        }
    }

    return data;
}