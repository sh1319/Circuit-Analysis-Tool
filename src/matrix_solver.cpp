#include "matrix.hpp"

using namespace std;
using namespace arma;

fmat solve_matrix(circuitmat cmat, circuit cir, vector<string> list_of_node, float time) {
    float pi = 3.14159265359;
    vector<string> node_list = remove_gnd_node(cir);
    
    // SECTION 1.
    // initialize all matrices
    if(time>=0) {
        cmat.gmat.zeros();
        cmat.imat.zeros();
        cmat.vmat.zeros();
    }
    
    // SECTION 2.
    // sort out main AC sources (value equal to "SINE")
    int position_of_v_at_mat, position_of_i_at_mat;
    for(int j=0; j<cir.components_list.size(); j++){
        // AC voltage source
        if(component_type(cir.components_list[j].name) == 'V' && cir.components_list[j].value == "SINE") { 
            string main_src_node = find_source_voltage_node(cir);
            for(unsigned i=0; i<node_list.size(); ++i) {
                if(main_src_node == node_list[i]) {
                    // find position of v source's node from a list of node
                    position_of_v_at_mat = i;
                }
            }
            // the element of conductance matrix at v source position = 1
            cmat.gmat.at(position_of_v_at_mat,position_of_v_at_mat) = 1;
            // the element of current matrix at v source position = AC source's instantaneous value
            cmat.imat.at(position_of_v_at_mat,0) = cir.voltages_src.amplitude*sin(2*pi*cir.voltages_src.frequency*time) + cir.voltages_src.dc_offset;
        }
        // AC current source
        else if(component_type(cir.components_list[j].name) == 'I' && cir.components_list[j].value == "SINE") {
            string main_src_node = find_source_current_node(cir);
            for(unsigned i=0; i<node_list.size(); ++i) {
                if(main_src_node == node_list[i]) {
                    // find position of i source's node from a list of node
                    position_of_i_at_mat = i;
                }
            }
            // the conductance matrix is not affected by current source
            // the element of current matrix at i source position = AC source's instantaneous value
            cmat.imat.at(position_of_i_at_mat,0) = cir.currents_src.amplitude*sin(2*pi*cir.currents_src.frequency*time) + cir.currents_src.dc_offset;
        }
    }
    
    //SECTION 3.
    // gmat fill in relevant elements and do not change or leave some elements at value zero for some cases outlined in code
	for(int i = 0; i<node_list.size(); i++) {
        if(position_of_i_at_mat>=0) {;}
        else {
            if(i == position_of_v_at_mat) {continue;} // continue to next iteration if this row is manipulated due to AC voltage source
        }
		for(int j = 0; j<node_list.size(); j++) {
			if(i != j) {
                // if two nodes have nodes in between
                if(abs(i-j) >= 2) {
                    // find all components that directly links two nodes (no intermediate nodes)
                    vector<component> node_betw = find_component(cir, list_of_node[i], list_of_node[j]);
                    // if there is any components linking those two nodes calculate the total conductance
                    if(!node_betw.empty()) {
                        for(unsigned k=0; k<node_betw.size(); ++k) {
                            // element G(i,j) is set as the total conductance 
                            cmat.gmat(i,j) = cmat.gmat(i,j) + find_conductance(node_betw[k]);
                        }
                        // the minus sign is put since it is not a principal diagonal element
                        cmat.gmat(i,j) = -cmat.gmat(i,j); 
                    }
                    // if there is no component linking two nodes directly G(i,j) = 0
                    else {continue;}
                }
                // if two nodes are consecutive / neighbouring / next to each other
                else {
                    // find all components between two consecutive/direct nodes
                    vector<component> components_betw_ij = find_component(cir, list_of_node[i], list_of_node[j]);
                    for(unsigned k=0; k<components_betw_ij.size(); ++k) {
                        cmat.gmat(i,j) = cmat.gmat(i,j) + find_conductance(components_betw_ij[k]);
                    }
                    // minus sign accounts for same reason (not a principal diagonal element)
                    cmat.gmat(i,j) = -cmat.gmat(i,j); 
                }
            }
            else {
                // when element in principal diagonal element, find all conductances connected to the node
                cmat.gmat(i,j) = find_node_conductance(cir, list_of_node[i]);
            }
		}
	}

    //SECTION 4.
    // sorting out dc sources
    vector<component> dc_sources, dc_sources_at_gnd;
    for(unsigned i=0; i<cir.components_list.size(); ++i) {
        // dc sources with non-ground nodes
        if(((component_type(cir.components_list[i].name) == 'V') 
        || (component_type(cir.components_list[i].name) == 'I')) 
        && (cir.components_list[i].value != "SINE") 
        && ((cir.components_list[i].nodeleft != "0") && (cir.components_list[i].noderight != "0"))) {
            dc_sources.push_back(cir.components_list[i]);
        }
        // dc sources with one of the nodes at ground
        else if (((component_type(cir.components_list[i].name) == 'V') 
        || (component_type(cir.components_list[i].name) == 'I')) 
        && (cir.components_list[i].value != "SINE") 
        && (((cir.components_list[i].nodeleft == "0") && (cir.components_list[i].noderight != "0")) 
        || ((cir.components_list[i].nodeleft != "0") && (cir.components_list[i].noderight == "0")) ) ) {
            dc_sources_at_gnd.push_back(cir.components_list[i]);
        }        
    }


    for(unsigned j=0; j<dc_sources.size(); ++j) {
        if(component_type(dc_sources[j].name) == 'V') {
            // supernode V simplify
            int right, left;
            string node_right, node_left;
            for(unsigned k=0; k<node_list.size(); ++k) {
                if(dc_sources[j].noderight == node_list[k]) {
                    right = k;
                    node_right = dc_sources[j].noderight;
                }
                if(dc_sources[j].nodeleft == node_list[k]) {
                    left = k;
                    node_left = dc_sources[j].nodeleft;
                }
            }
            cmat.gmat.at(left,right) = -1;
            cmat.gmat.at(left,left) = 1;
            cmat.imat.at(left,0) = dc_sources[j].floatvalue;
            cmat.gmat.at(right,left) = find_node_conductance(cir, node_left);
        }
        else if(component_type(dc_sources[j].name) == 'I') {
            // simplify I source
            int right, left;
            for(unsigned p=0; p<node_list.size(); ++p) {
            }
            for(unsigned k=0; k<node_list.size(); ++k) {
                if(k == position_of_v_at_mat) {continue;}
                if(dc_sources[j].noderight == node_list[k]) {
                    right = k;
                    cmat.imat.at(right-1,0) = -dc_sources[j].floatvalue;
                }
                if(dc_sources[j].nodeleft == node_list[k]) {
                    left = k;
                    cmat.imat.at(left,0) = dc_sources[j].floatvalue;
                }
            }
        }
        else {
            assert(0); // this should not happen
        }
    }


    for(unsigned j=0; j<dc_sources_at_gnd.size(); ++j) {
        if(component_type(dc_sources_at_gnd[j].name) == 'V') {
            // supernode V simplify
            int row;
            string node_right;
            for(unsigned k=0; k<list_of_node.size(); ++k) {
                if(dc_sources_at_gnd[j].noderight == "0") {
                    for(unsigned p=0; p<list_of_node.size(); ++p) {
                        if(list_of_node[p] == dc_sources_at_gnd[j].nodeleft) {
                            row = p;
                        }
                    }
                    for(unsigned l=0; l<list_of_node.size(); ++l) {
                        cmat.gmat.at(row,l) = 0;
                    }
                    cmat.gmat.at(row,row) = 1;
                    cmat.imat.at(row,0) = dc_sources_at_gnd[j].floatvalue;
                }
                if(dc_sources_at_gnd[j].nodeleft == "0") {
                    for(unsigned p=0; p<list_of_node.size(); ++p) {
                        if(list_of_node[p] == dc_sources_at_gnd[j].noderight) {
                            row = p;
                        }
                    }
                    for(unsigned l=0; l<list_of_node.size(); ++l) {
                        cmat.gmat.at(row,l) = 0;
                    }
                    cmat.gmat.at(row,row) = 1;
                    cmat.imat.at(row,0) = -dc_sources_at_gnd[j].floatvalue;
                }
            }
        }
        else if(component_type(dc_sources_at_gnd[j].name) == 'I') {
            // simplify I source
            int row;
            for(unsigned k=0; k<list_of_node.size(); ++k) {
                if(dc_sources_at_gnd[j].noderight == "0") {
                    for(unsigned p=0; p<list_of_node.size(); ++p) {
                        if(list_of_node[p] == dc_sources_at_gnd[j].nodeleft) {
                            row = p;
                        }
                    }
                    cmat.imat.at(row,0) = dc_sources_at_gnd[j].floatvalue;
                }
                if(dc_sources_at_gnd[j].nodeleft == "0") {
                    for(unsigned p=0; p<list_of_node.size(); ++p) {
                        if(list_of_node[p] == dc_sources_at_gnd[j].noderight) {
                            row = p;
                        }
                    }
                    cmat.imat.at(row,0) = -dc_sources_at_gnd[j].floatvalue;
                }
            }
        }
        else {
            assert(0); // this should not happen
        }
    }
    
    // calculate vmat at this instant of time

    
    //SECTION 5.
    //calculation of vmat
    fmat inv_gmat = inv(cmat.gmat);

    cmat.vmat = inv_gmat * cmat.imat;

    return cmat.vmat;
}