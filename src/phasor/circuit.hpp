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
	complex<float> conductance;
};

struct sim_time { 	// simulation time: all time related values from e.g. (.tran 0 0.01 0 0.001) 
	float start;
	float stop;
	float save_data;
	float max_step;
};

struct source_v {				// source voltage values
	float dc_offset;
	float amplitude;
	float frequency;
};

struct circuit {
	vector<component> components_list;
	vector<int> number_of_each_component; // [0]:V, [1]:R, [2]:C, [3]:L, [4]:D. 
	source_v voltages_src;
	sim_time simulation;
	vector<string> node_list;
};

circuit netlist_to_components(); //convert netlist txt file to a vector<component>
void print_circuit_values(circuit cir); //cout all values in circuit
float string_value_to_float(string value); //convert string values to float by converting n = 10^-9, etc.
char component_type(string name); //returns component type: R, L, C, V
complex<float> find_conductance(component comp, float frequency);
vector<component> find_component(circuit cir, string nodeleft, string noderight);
vector<component> find_node_component(circuit cir, string node);
vector<string> find_node_list(circuit cir);
complex<float> find_node_conductance(circuit cir, string node);
vector<int> get_number_of_each_component(circuit cir);
int number_of_l_and_c(circuit cir);
bool is_parallel(circuit cir, component comp1, component comp2);

#endif
