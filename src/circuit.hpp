#ifndef circuit_hpp
#define circuit_hpp
 
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <complex>

using namespace std;

struct component {
	string name;
	string value;
	float floatvalue;
	string nodeleft;
	string noderight;
	float charge_c;
	float current_c;
	float flux_l;
	float voltage_l;
	float conductance;
	float voltage_d;
	float current_at_time;
};

struct sim_time { 	// simulation time: all time related values from e.g. (.tran 0 0.01 0 0.001) 
	float start;
	float stop;
	float save_data;
	float max_step;
};

struct source_v_i {				// source voltage and current source values
	float dc_offset;
	float amplitude;
	float frequency;
};

struct circuit {
	vector<component> components_list;
	source_v_i voltages_src;
	source_v_i currents_src;
	sim_time simulation;
	vector<string> node_list;
};

circuit netlist_to_circuit(); //convert netlist txt file to a vector<component>
void print_circuit_values(circuit cir); //cout all values in circuit
float string_value_to_float(string value); //convert string values to float by converting n = 10^-9, etc.
char component_type(string name); //returns component type: R, L, C, V
string component_type_LCD(string name);
float find_conductance(component comp);
bool dc_current_src_presence(circuit cir, string nodeleft, string noderight);
vector<component> find_component(circuit cir, string nodeleft, string noderight);
vector<component> find_node_component(circuit cir, string node);
vector<string> find_node_list(circuit cir);
float find_node_conductance(circuit cir, string node);

// simplifying circuits by combining same types of components

#endif