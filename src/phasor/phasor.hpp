#ifndef phasor_hpp
#define phasor_hpp

#include "circuit.hpp"

#include <iostream>
#include <complex>
#include <cmath>

using namespace std;

float phasor_at_t_src_v(complex<float> phasor, float frequency, float time, float dc);
float phasor_at_t_other_nodes(complex<float> phasor, float frequency, float time);
complex<float> sine_to_phasor(float amplitude, float dc);
float get_time_constant(circuit cir);
float first_order_transient(complex<float> phasor, float time, float time_constant);
float second_order_transient(complex<float> phasor, float time, float R, float L, float C);

#endif