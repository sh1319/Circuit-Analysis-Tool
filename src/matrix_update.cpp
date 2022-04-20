#include "matrix.hpp"

using namespace std;
using namespace arma;

circuit circuit_to_simplified_circuit_zero(circuit cir, circuitmat cmat){
    circuit simp_cir;
    for(int i=0; i<cir.components_list.size(); ++i) {
    component filtercomp;
    // AC V & I source -> time = 0 -> dc offset
    // name, value, floatvalue, nodeleft, noderight
    if(component_type(cir.components_list[i].name) == 'V' && cir.components_list[i].value == "SINE"){
        filtercomp.name = cir.components_list[i].name;
        filtercomp.floatvalue = cir.voltages_src.dc_offset;
        filtercomp.nodeleft = cir.components_list[i].nodeleft;
        filtercomp.noderight = cir.components_list[i].noderight;
        filtercomp.value = "SINE";
        simp_cir.components_list.push_back(filtercomp);
        }
        else if(component_type(cir.components_list[i].name) == 'I' && cir.components_list[i].value == "SINE"){
            filtercomp.name = cir.components_list[i].name;
            filtercomp.floatvalue = cir.currents_src.dc_offset;
            filtercomp.nodeleft = cir.components_list[i].nodeleft;
            filtercomp.noderight = cir.components_list[i].noderight;
            filtercomp.value = "SINE";
            simp_cir.components_list.push_back(filtercomp);
        }
        else if(component_type(cir.components_list[i].name) == 'L'){
            filtercomp.name = "I" + cir.components_list[i].name;        //IL
            filtercomp.floatvalue = 0;
            filtercomp.voltage_l = voltage_across_inductor(cir, cmat, cir.components_list[i].nodeleft, cir.components_list[i].noderight);
            filtercomp.flux_l = 0;
            filtercomp.value = cir.components_list[i].value;
            filtercomp.nodeleft = cir.components_list[i].nodeleft;
            filtercomp.noderight = cir.components_list[i].noderight;
            simp_cir.components_list.push_back(filtercomp);
        }
        else if(component_type(cir.components_list[i].name) == 'C'){
            filtercomp.name = "V" + cir.components_list[i].name;        //VC
            filtercomp.floatvalue = 0;
            filtercomp.current_c = current_betw_consec_nodes(cir.components_list[i], cir, cmat, cir.components_list[i].nodeleft, cir.components_list[i].noderight);
            filtercomp.charge_c = 0;
            filtercomp.value = cir.components_list[i].value;
            filtercomp.nodeleft = cir.components_list[i].nodeleft;
            filtercomp.noderight = cir.components_list[i].noderight;
            simp_cir.components_list.push_back(filtercomp);
        }
        else if(component_type(cir.components_list[i].name) == 'D') {
            filtercomp.name = "V" + cir.components_list[i].name;        //VC
            filtercomp.floatvalue = 0.35;
            filtercomp.value = cir.components_list[i].value;
            filtercomp.nodeleft = cir.components_list[i].nodeleft;
            filtercomp.noderight = cir.components_list[i].noderight;
            simp_cir.components_list.push_back(filtercomp);
        }
        else{
            simp_cir.components_list.push_back(cir.components_list[i]);
        }
    }
    simp_cir.simulation = cir.simulation;
    simp_cir.node_list = cir.node_list;
    simp_cir.voltages_src = cir.voltages_src;
    simp_cir.currents_src = cir.currents_src;
    return simp_cir;
}

circuit circuit_to_simplified_circuit(circuit prevcir, circuitmat prevcmat, float time, float timestep){
    float pi = 3.14159265359;
    circuit simp_cir;
    circuitmat cmat;
     for(int i=0; i<prevcir.components_list.size(); ++i) {
        component filtercomp;
        if(component_type(prevcir.components_list[i].name) == 'V' && prevcir.components_list[i].value == "SINE"){
                filtercomp.name = prevcir.components_list[i].name;
                filtercomp.floatvalue = prevcir.voltages_src.amplitude*sin(2*pi*prevcir.voltages_src.frequency*time)+prevcir.voltages_src.dc_offset;
                filtercomp.nodeleft = prevcir.components_list[i].nodeleft;
                filtercomp.noderight = prevcir.components_list[i].noderight;
                filtercomp.value = "SINE";
                simp_cir.components_list.push_back(filtercomp);
        }
        else if(component_type(prevcir.components_list[i].name) == 'I' && prevcir.components_list[i].value == "SINE"){
                filtercomp.name = prevcir.components_list[i].name;
                filtercomp.floatvalue = prevcir.currents_src.amplitude*sin(2*pi*prevcir.currents_src.frequency*time)+prevcir.currents_src.dc_offset;
                filtercomp.nodeleft = prevcir.components_list[i].nodeleft;
                filtercomp.noderight = prevcir.components_list[i].noderight;
                filtercomp.value = "SINE";
                simp_cir.components_list.push_back(filtercomp);
        }
        else if((component_type_LCD(prevcir.components_list[i].name) == "IL")){
            // use previous voltage to find the extra flux accumulated on inductor. this flux is used to calculate the value of i source by dividing by inductance
            filtercomp.voltage_l = voltage_across_inductor(prevcir, prevcmat, prevcir.components_list[i].nodeleft, prevcir.components_list[i].noderight);
            float extra_flux = prevcir.components_list[i].voltage_l * timestep;
            float inductance = string_value_to_float(prevcir.components_list[i].value);
            filtercomp.flux_l = prevcir.components_list[i].flux_l + extra_flux;
            filtercomp.floatvalue = -filtercomp.flux_l / inductance;
            filtercomp.value = prevcir.components_list[i].value;
            filtercomp.nodeleft = prevcir.components_list[i].nodeleft;
            filtercomp.noderight = prevcir.components_list[i].noderight;
            filtercomp.name = prevcir.components_list[i].name;
            simp_cir.components_list.push_back(filtercomp);
        }
        else if((component_type_LCD(prevcir.components_list[i].name) == "VC")){

            // use previous current to find the extra charge accumulated on capacitor. this charge is used to calculate the value of v source by dividing by capacitance
            filtercomp.current_c = current_betw_consec_nodes(prevcir.components_list[i], prevcir, prevcmat, prevcir.components_list[i].nodeleft, prevcir.components_list[i].noderight);
            float extra_charge = prevcir.components_list[i].current_c * timestep;
            float capacitance = string_value_to_float(prevcir.components_list[i].value);
            if(prevcir.components_list[i].current_c == 0) {
                filtercomp.charge_c = prevcir.components_list[i].charge_c;
                if(filtercomp.charge_c == 0) {
                    filtercomp.floatvalue = prevcir.components_list[i].floatvalue;
                }
                filtercomp.floatvalue = filtercomp.charge_c / capacitance;
            }
            else {
                filtercomp.charge_c = prevcir.components_list[i].charge_c + extra_charge;
                filtercomp.floatvalue = filtercomp.charge_c / capacitance;
            }
            filtercomp.value = prevcir.components_list[i].value;
            filtercomp.nodeleft = prevcir.components_list[i].nodeleft;
            filtercomp.noderight = prevcir.components_list[i].noderight;
            filtercomp.name = prevcir.components_list[i].name;
            simp_cir.components_list.push_back(filtercomp);
        }
        else {
                simp_cir.components_list.push_back(prevcir.components_list[i]);
        }
    }
    simp_cir.simulation = prevcir.simulation;
    simp_cir.node_list = prevcir.node_list;
    simp_cir.voltages_src = prevcir.voltages_src;
    simp_cir.currents_src = prevcir.currents_src;
    return simp_cir;
}