#include "phasor.hpp"

#include <vector>

using namespace std;

void phasor_at_t_test() {
    vector<complex<float>> phasor = {{1,1},{2,4},{2,1}};
    vector<float> time = { 0.0001, 0.001, 0.01, 0.1};
    for(unsigned i=0; i<phasor.size(); ++i) {
        for(unsigned j=0; j<time.size(); ++j) {
            float value = phasor_at_t(phasor[i], 10, time[j]);
            cout << "Time: " << time[j] << " Value of phasor " << phasor[i].real() << "+" << phasor[i].imag() << "j:" << value << " " << endl;
        }
    }
}

void sine_to_phasor_test() {
    // 5sin(wt + 0) + 0
    float amplitude = 5;
    float dc_offset = 0;
    cout << sine_to_phasor(amplitude, dc_offset) << endl;
}

int main() {
    phasor_at_t_test();
    sine_to_phasor_test();
    cout << phasor_at_t({0,-5}, 10, 0.001) << endl;
    cout << 5*sin(10*0.001*2*3.141592) << endl;
    return 0;
}