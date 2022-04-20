#include <iostream>
#include "circuit.hpp"

using namespace std;

// netlist input to data structure circuit
circuit netlist_to_circuit(){
    component comp;
    circuit cir;
    source_v_i src_v, src_i;
    string filter, bin;
    char s;
    while(1){
		cin >> filter;
		if(filter ==".backanno") {break;}
		else if(filter == "*") {cin >> bin >> comp.name;}
		else if(filter == ".tran") {
			s = cin.get(); //gets rid of a single whitespace;
			cin >> cir.simulation.start >> cir.simulation.stop >> cir.simulation.save_data >> cir.simulation.max_step; // reads in 4 time values
			break;
		}
        else if(filter == ".model") {
            cin >> bin >> bin >> bin >> bin;
            continue;
        }
		else if(filter == ".end") {break;}
		else {comp.name = filter;}
		cin >> comp.nodeleft >> comp.noderight;
        cin >> filter;
        src_v.frequency = 0;
        src_i.frequency = 0;
        if((component_type(comp.name) == 'V')&&(filter.substr(0,4) == "SINE")) {
            src_v.dc_offset = stof(filter.substr(5));
            s = cin.get();
            cin >> src_v.amplitude;
            cin >> bin;
            src_v.frequency = string_value_to_float(bin.substr(0,bin.length()-1));
            cir.voltages_src = src_v;
            comp.value = "SINE";
        }
        else if((component_type(comp.name) == 'I')&&(filter.substr(0,4) == "SINE")) {
            src_i.dc_offset = stof(filter.substr(5));
            s = cin.get();
            cin >> src_i.amplitude;
            cin >> bin;
            src_i.frequency = string_value_to_float(bin.substr(0,bin.length()-1));
            cir.currents_src = src_i;
            comp.value = "SINE";
        }
        else if((component_type(comp.name) == 'D')) {
            comp.value = filter;
        }
        else {
            comp.value = (filter);
            comp.floatvalue = string_value_to_float(comp.value);
            if(component_type(comp.name) == 'C') {
                comp.charge_c = 0;
                comp.current_c = 0;
            }
            if(component_type(comp.name) == 'L') {
                comp.flux_l = 0;
                comp.voltage_l = 0;
            }
           // comp.conductance = find_conductance(comp);
        }
		cir.components_list.push_back(comp);
	}
    for(unsigned k=0; k<cir.components_list.size(); ++k) {
        if(component_type(cir.components_list[k].name) == 'R') {
            cir.components_list[k].conductance = find_conductance(cir.components_list[k]);
        }
    }
	cir.node_list = find_node_list(cir);
    return cir;
}

// print data structure circuit values for checking
void print_circuit_values(circuit cir) {
    cout << "Components list: " << endl;
    //this part prints out the components again, just to check it was read well
	for(int j=0; j<cir.components_list.size(); j++){
        int count = 0;
		cout << cir.components_list[j].name << " " << cir.components_list[j].nodeleft << " " << cir.components_list[j].noderight << " ";
        if(component_type(cir.components_list[j].name) == 'V' && cir.components_list[j].value == "SINE") { 
            cout << "Source Voltage" << endl;
        }
        else if(component_type(cir.components_list[j].name) == 'I' && cir.components_list[j].value == "SINE") { 
            cout << "Source Current" << endl;
        }
        else if(component_type(cir.components_list[j].name) == 'D') {
            cout << "Silicon Diode" << endl;
        }
        else {
            cout << cir.components_list[j].floatvalue << " ";
            if(component_type(cir.components_list[j].name) != 'V' && component_type(cir.components_list[j].name) != 'I') {
                cout << cir.components_list[j].conductance << endl;
            }
            else {cout << endl;}
        }
	}
    cout << endl;

	//check time related info
	cout << "Start Time: " << cir.simulation.start << endl;
    cout << "Stop Time: " << cir.simulation.stop << endl;
    cout << "Start Saving Data Time: " << cir.simulation.save_data << endl;
    cout << "Maximum Timestep: " << cir.simulation.max_step << endl;
    cout << endl;

    //check source voltage values
    for(int j=0; j<cir.components_list.size(); j++){
        if(component_type(cir.components_list[j].name) == 'V' && cir.components_list[j].value == "SINE") { 
            cout << "Source Voltage"<< endl;
            cout << "DC offset: " << cir.voltages_src.dc_offset << endl;
            cout << "Amplitude: " << cir.voltages_src.amplitude << endl;
            cout << "Frequency: " << cir.voltages_src.frequency << " Hz" << endl << endl;
        }
        else if(component_type(cir.components_list[j].name) == 'I' && cir.components_list[j].value == "SINE"){
            cout << "Source Current"<< endl;
            cout << "DC offset: " << cir.currents_src.dc_offset << endl;
            cout << "Amplitude: " << cir.currents_src.amplitude << endl;
            cout << "Frequency: " << cir.currents_src.frequency << " Hz" << endl <<endl;
        }
    }

	cout << "Nodes:";
	for(int i=0; i<cir.node_list.size(); i++){
		cout << cir.node_list[i] << " ";
	}
    cout << endl;
}

// converting component string value to float e.g. 1k -> 1000
float string_value_to_float(string value) {
    float num;
    if(value[value.length()-1] == ')') {
        if(value[value.length()-2] == 'k' || value[value.length()-2] == 'K') {
            num = stof(value.substr(0, value.length()-1));
            num *= 1000;
        }
    }
    else if(value[value.length()-1] == 'k' || value[value.length()-1] == 'K') {
            num = stof(value.substr(0, value.length()-1));
            num *= 1000;
    }
    else if(value[value.length()-1] == 'n') {
        num = stof(value.substr(0, value.length()-1));
        num *= 0.000000001;
    }
    else if(value[value.length()-1] == 'm') {
        num = stof(value.substr(0, value.length()-1));
        num *= 0.001;
    }
    else {
        num = stof(value.substr(0, value.length()));
    }
    return num;
}
 
// extract component type from string name of component
char component_type(string name) {
    char actual_type = name[0];
    return actual_type;
}

string component_type_LCD(string name){
    return name.substr(0,2);
}

// find a sum of conductances at a node
float find_node_conductance(circuit cir, string node){
	float nodeconductance = 0;	
	vector<component> complist;
		complist = find_node_component(cir, node);
		for(unsigned i=0; i<complist.size(); ++i) {
			nodeconductance += find_conductance(complist[i]);
		}
	return nodeconductance;
}

// find all components connected to the node except for V sources
// **TODO: change this so that it only disregards Source Voltage not DC Voltage
vector<component> find_node_component(circuit cir, string node){
	vector<component> complist;
	 for(unsigned i=0; i<cir.components_list.size(); ++i) {
		if((cir.components_list[i].nodeleft == node || cir.components_list[i].noderight == node) && !(component_type(cir.components_list[i].name) == 'V'||component_type(cir.components_list[i].name) == 'V')){
			complist.push_back(cir.components_list[i]);
		}
	 }
	return complist;
}

// find conductance given a component name 
float find_conductance(component comp) {
    float pi = 3.14159265359;
    float one_float = 1;
    float conductance;
    if(component_type(comp.name) == 'R') {
        if(comp.value == "inf") {
            conductance = 0;
            return conductance;
        }
        else {
            conductance = one_float/comp.floatvalue;
        }
    }
    else{
        conductance = 0;
    }
    return conductance;
}

// find all components between two nodes (TODO?? assumed no nodes in between)
vector<component> find_component(circuit cir, string nodeleft, string noderight) {
    vector<component> parallel_comp;
    for(unsigned i=0; i<cir.components_list.size(); ++i) {
        if((cir.components_list[i].nodeleft == nodeleft && cir.components_list[i].noderight == noderight) || (cir.components_list[i].nodeleft == noderight && cir.components_list[i].noderight == nodeleft)) {
            parallel_comp.push_back(cir.components_list[i]);
        }
    }
    return parallel_comp;
}

// find node list ****RANDOM ORDER (the order given by the netlist) -> could be improved by having rules to order them in certain way
vector<string> find_node_list(circuit cir){
    cir.node_list.push_back(cir.components_list[0].nodeleft);
    cir.node_list.push_back(cir.components_list[0].noderight);
    for(int j=1; j<cir.components_list.size(); j++){
        int count1 = 0;
        int count2 = 0;
        for(int i=0; i<cir.node_list.size(); i++){
            if(cir.node_list[i] != cir.components_list[j].nodeleft){
                count1++;
            }
            if(count1 == cir.node_list.size()){
                cir.node_list.push_back(cir.components_list[j].nodeleft);
            }
            if(cir.node_list[i] != cir.components_list[j].noderight){
                count2++;
            }
            if(count2 == cir.node_list.size()){
                cir.node_list.push_back(cir.components_list[j].noderight);
            }
        }
    }
    return cir.node_list;
}