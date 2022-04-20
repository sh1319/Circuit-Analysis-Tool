#!bin/bash 

set -e

g++ phasor.hpp phasor.cpp phasor_test.cpp -o phasor
./phasor

