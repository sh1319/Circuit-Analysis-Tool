#include "matrix.hpp"
#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

fmat find_current(circuit cir, circuitmat cmat){
    fmat current_of_comp;
    current_of_comp.zeros(cir.components_list.size(),1);
    for(int i = 0; i < cir.components_list.size(); i++){
        float current_through_comp = 0;
        if(component_type_LCD(cir.components_list[i].name) == "VC"){
            current_through_comp = current_betw_consec_nodes(cir.components_list[i], cir, cmat, cir.components_list[i].nodeleft, cir.components_list[i].noderight);
            cir.components_list[i].current_at_time = current_through_comp;
        }
        else if(component_type_LCD(cir.components_list[i].name) == "IL"){

            current_through_comp = cir.components_list[i].floatvalue;
            cir.components_list[i].current_at_time = -current_through_comp;
        }
        else if(component_type(cir.components_list[i].name) == 'V' && !(component_type_LCD(cir.components_list[i].name) == "VC")){
            current_through_comp = current_betw_consec_nodes(cir.components_list[i], cir, cmat, cir.components_list[i].nodeleft, cir.components_list[i].noderight);
            cir.components_list[i].current_at_time = current_through_comp;
        }
        else if(component_type(cir.components_list[i].name) == 'I' && !(component_type_LCD(cir.components_list[i].name) == "IL")){
            current_through_comp = cir.components_list[i].floatvalue;
            cir.components_list[i].current_at_time = current_through_comp;
        }
        else if(component_type(cir.components_list[i].name) == 'R'){
            float voltageright , voltageleft;
            vector<string> node_list = remove_gnd_node(cir);
                for(unsigned j=0; j<node_list.size(); ++j) {
                    if(node_list[j] == cir.components_list[i].nodeleft) {
                        voltageleft = cmat.vmat(j,0);
                    }
                    if(node_list[j] == cir.components_list[i].noderight) {
                        voltageright = cmat.vmat(j,0);
                    }
                }
            current_through_comp = (voltageleft-voltageright)/cir.components_list[i].floatvalue;
            cir.components_list[i].current_at_time = current_through_comp;
        }
        else if(component_type_LCD(cir.components_list[i].name) == "RD"){
            // dont know yet TO DO
            cir.components_list[i].current_at_time = current_through_comp;
        }
        else if(component_type_LCD(cir.components_list[i].name) == "VD"){
            // dont know yet TO DO
            cir.components_list[i].current_at_time = current_through_comp;
        }
        else{
            ;
        }
        current_of_comp.at(i,0) = cir.components_list[i].current_at_time;
    }
    return current_of_comp;
}