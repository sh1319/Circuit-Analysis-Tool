#include "phasor.hpp"

using namespace std;
float pi = 3.14159265359;


// STEADY-STATE
// PHASOR TO CONTINUOUS
float phasor_at_t_src_v(complex<float> phasor, float frequency, float time, float dc) {
    float x = phasor.real();
    float y = phasor.imag();
    float omega = 2*pi*frequency;
    float amplitude = sqrt(pow(x,2) + pow(y,2));
    float phase = atan2(y,x);
    float value = amplitude*cos(omega*time+phase) + dc;
    return value;
}

float phasor_at_t_other_nodes(complex<float> phasor, float frequency, float time) {
    float x = phasor.real();
    float y = phasor.imag();
    float omega = 2*pi*frequency;
    float amplitude = sqrt(pow(x,2) + pow(y,2));
    float phase = atan2(y,x);
    float value = amplitude*cos(omega*time+phase);
    return value;
}

// CONTINUOUS TO PHASOR
complex<float> sine_to_phasor(float amplitude, float dc) {
    float x = 0;
    float y = -amplitude;
    complex<float> phasor = {x, y};
    return phasor;
}



// TRANSIENT
// **TODO
//Capacitors and Inductors Differential Equations
//float 

// **TODO
// Get Time constant from circuit
float get_time_constant(circuit cir) {
    float time_constant;
    int count = 0;
    int size = cir.components_list.size();
    vector<int> number_of_comp = get_number_of_each_component(cir); //v,r,l,c,d
    if (size <= 3) {
        float R = 0;
        float L = 0;
        float C = 0;
        for(unsigned i=0; i<size; ++i) {
            if(component_type(cir.components_list[i].name) == 'L') {
                L = cir.components_list[i].floatvalue;
            }
            if(component_type(cir.components_list[i].name) == 'C') {
                C = cir.components_list[i].floatvalue;
            }
            if(component_type(cir.components_list[i].name) == 'R') {
                R = cir.components_list[i].floatvalue;
            }
            if(C == 0) {
                if(R == 0) {time_constant = 0;}
                else {time_constant = L/R;}
            }
            else if (L == 0) {
                time_constant = R*C;
            }
            else {
                assert(0);
            }
        }
    }
    else {
        if(number_of_l_and_c(cir) < 2) {
            // simplify circuit first
            // TODO
            get_time_constant(cir);
        }
        else {
            assert(0);
        }
    }
    return time_constant;
}

// **TODO
// Get first order transient term
float first_order_transient(complex<float> phasor, float time, float time_constant) {
    float A = -phasor_at_t_other_nodes(phasor, 0, 0);
    float y = A*exp(-time/time_constant);
    return y;
}

float second_order_transient(complex<float> phasor, float time, float R, float L, float C) {
    float alpha, resonant_f, s1, s2, A1, A2;
    float one_float = 1;
    // if series RLC
    alpha = R/(2*L);
    resonant_f = one_float/(L*C); 

    // if parallel RLC
    /*
    alpha = one_float/(2*R*C);
    resonant_f = one_float/(L*C);
    */
    if (alpha > resonant_f) {
        // over-damped
        s1 = - alpha + sqrt(alpha*alpha-resonant_f*resonant_f);
        s2 = - alpha - sqrt(alpha*alpha-resonant_f*resonant_f);
    }
    else if (alpha == resonant_f) {
        // critically-damped
        ;
    }
    else {
        // under-damped
        ;
    }



    A1 = -phasor_at_t_other_nodes(phasor, 0, 0);
    A2 = -phasor_at_t_other_nodes(phasor, 0, 0);

    return (A1*exp(s1*time) + A2*exp(s2*time));
}
