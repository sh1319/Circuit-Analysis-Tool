#include <iostream>
#include "circuit.hpp"

using namespace std;


// netlist input to data structure circuit
circuit netlist_to_components(){
    component comp;
    circuit cir;
    source_v src_v;
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
		else if(filter == ".end") {break;}
		else {comp.name = filter;}
		cin >> comp.nodeleft >> comp.noderight;
        cin >> filter;
        //cerr << "I'm in else" << endl;
        if(filter.substr(0,4) == "SINE" || filter.substr(0,4) == "sine") {
            src_v.dc_offset = stof(filter.substr(5));
            s = cin.get();
            cin >> src_v.amplitude;
            cin >> bin;
            src_v.frequency = string_value_to_float(bin.substr(0,bin.length()-1));
            cir.voltages_src = src_v;
            comp.value = "SINE";
        }
        else {
            comp.value = (filter);
            comp.floatvalue = string_value_to_float(comp.value);
           // comp.conductance = find_conductance(comp);
        }
		cir.components_list.push_back(comp);
	}
    for(unsigned k=0; k<cir.components_list.size(); ++k) {
        if(component_type(cir.components_list[k].name) == 'R' || component_type(cir.components_list[k].name) == 'C' || component_type(cir.components_list[k].name) == 'L') {
            cir.components_list[k].conductance = find_conductance(cir.components_list[k], cir.voltages_src.frequency);
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
        else {
            cout << cir.components_list[j].floatvalue << " ";
            if(component_type(cir.components_list[j].name) != 'V' && component_type(cir.components_list[j].name) != 'I') {
                cout << "conductance: " << cir.components_list[j].conductance << endl;
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
    //cout << "size" << cir.voltages_src.size() << endl;
    //for(unsigned i=0; i<cir.voltages_src.size(); ++i) {
    cout << "Source Voltage"<< endl;
    cout << "DC offset: " << cir.voltages_src.dc_offset << endl;
    cout << "Amplitude: " << cir.voltages_src.amplitude << endl;
    cout << "Frequency: " << cir.voltages_src.frequency << " Hz" << endl;
    //}

	cout << endl << "Nodes: ";
	for(int i=0; i<cir.node_list.size(); i++){
		cout << cir.node_list[i] << " ";
	}
    cout << endl;
    
    for(unsigned i=0; i<cir.node_list.size(); i++) {
        cout << "Components at Node " << cir.node_list[i] << ": ";
        vector<component> comp_at_node = find_node_component(cir, cir.node_list[i]);
        for(unsigned j=0; j<comp_at_node.size(); ++j) {
            cout << comp_at_node[j].name << " ";
        }
        cout << endl;
        cout << "Conductance at a Node " << cir.node_list[i] << ": " << find_node_conductance(cir, cir.node_list[i]) << endl;
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

// find a sum of conductances at a node
complex<float> find_node_conductance(circuit cir, string node){
	complex<float> node_conductance = {0,0};	
	vector<component> complist;
		complist = find_node_component(cir, node);
		for(unsigned i=0; i<complist.size(); ++i) {
			node_conductance += find_conductance(complist[i], cir.voltages_src.frequency);
		}
	return node_conductance;
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
complex<float> find_conductance(component comp, float frequency) {
    float pi = 3.14159265359;
    float one_float = 1;
    complex<float> conductance;
    complex<float> impedance;
    if(component_type(comp.name) == 'R' || component_type(comp.name) == 'r') {
        impedance = {comp.floatvalue, 0};
        conductance = one_float/impedance;
    }
    else if (component_type(comp.name) == 'C' || component_type(comp.name) == 'c') {
        impedance = {0, -1/(2*pi*frequency*comp.floatvalue)};
        conductance = one_float/impedance;
    }
    else if(component_type(comp.name) == 'L' || component_type(comp.name) == 'l') {
        impedance = {0, (2*pi*frequency*comp.floatvalue)};
        conductance = one_float/impedance;
    }
    else {
        assert(0);
        cerr << comp.name << "conductance undefined" << endl;
    }
    return conductance;
}

// find all components between two nodes (no nodes in between)
vector<component> find_component(circuit cir, string nodeleft, string noderight) {

    vector<component> monster_rat;

    if(cir.node_list.size() <= 10) ///SHOULD BE 3 not 10
	{
        	for(unsigned i=0; i<cir.components_list.size(); ++i)
		{
            		if((cir.components_list[i].nodeleft == nodeleft 
			&& cir.components_list[i].noderight == noderight) 
			|| (cir.components_list[i].nodeleft == noderight 
			&& cir.components_list[i].noderight == nodeleft)) 
			{
                		monster_rat.push_back(cir.components_list[i]);
         		}
       		 }
    	}
    else {
        ; // IMPLEMENT LATER
    }

    return monster_rat;

}

// find the index of component on the component list
int find_i(circuit cir, string nodeleft, string noderight) {
    if(cir.node_list.size() <= 3) {
        for(int i=0; i<cir.components_list.size(); ++i) {
           if((cir.components_list[i].nodeleft == nodeleft 
		&& cir.components_list[i].noderight == noderight) 
		|| (cir.components_list[i].nodeleft == noderight 
		&& cir.components_list[i].noderight == nodeleft)) 
		 {
                	return i;
               	}
        }
    }
    else {
        ; // IMPLEMENT LATER
    }
}

// find node list
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


// ==================================================
// Number of components return
vector<int> get_number_of_each_component(circuit cir) {
    int v = 0;
    int r = 0;
    int l = 0;
    int c = 0;
    int d = 0;
    vector<int> number_of_comp = {v,r,l,c,d};
    for(unsigned i=0; i<cir.components_list.size(); ++i) {
        if(component_type(cir.components_list[i].name) == 'V') {
            number_of_comp[0]++;
        }
        if(component_type(cir.components_list[i].name) == 'R') {
            number_of_comp[1]++;
        }
        if(component_type(cir.components_list[i].name) == 'L') {
            number_of_comp[2]++;
        }
        if(component_type(cir.components_list[i].name) == 'C') {
            number_of_comp[3]++;
        }
        if(component_type(cir.components_list[i].name) == 'D') {
            number_of_comp[4]++;
        }
    }
    for(unsigned j=0; j<5; ++j) {
        cir.number_of_each_component.push_back(number_of_comp[j]);
    }
    return cir.number_of_each_component;
}

// number of inductors and capacitors = order of the circuit
int number_of_l_and_c(circuit cir) {
    vector<int> num = get_number_of_each_component(cir);
    return (num[2] + num[3]);
}

// tells if component 1 and component 2 are parallel
bool is_parallel(circuit cir, component comp1, component comp2) {
    vector<component> parallel_list = find_component(cir, comp1.nodeleft, comp1.noderight);
    for(unsigned k=0; k<parallel_list.size(); ++k) {
        if(parallel_list[k].name == comp2.name) {return true;}
    }
    return false;
}