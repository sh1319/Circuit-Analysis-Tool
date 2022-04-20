#include <iostream>
#include "circuit.hpp"


using namespace std;

int main() {
	circuit cir = netlist_to_components();
	print_circuit_values(cir);
	return 0;
} 

