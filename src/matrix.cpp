#include "matrix.hpp"

using namespace std;
using namespace arma;

// circuit data structure to circuit matrix data structure
circuitmat circtocircmat(circuit cir){
	circuitmat circmat;
	circmat.numnode = cir.node_list.size();
    circmat.gmat.zeros(circmat.numnode-1,circmat.numnode-1);
    circmat.imat.zeros(circmat.numnode-1,1);
    circmat.vmat.zeros(circmat.numnode-1,1);
    return circmat;
}

// remove ground node from node_list as the output for the ground node will always be 0
vector<string> remove_gnd_node(circuit cir) {
    vector<string> nodes;
    for(unsigned j=0; j<cir.node_list.size(); j++) {
        if(cir.node_list[j] != "0") {
            nodes.push_back(cir.node_list[j]);
        }
    }
    return nodes;
}

// find which node is directly connected to the AC V source
string find_source_voltage_node(circuit cir){
    string returnnode;
    for(int j=0; j<cir.components_list.size(); j++) {
        if((cir.components_list[j].value == "SINE")&&(component_type(cir.components_list[j].name)=='V')){
                returnnode=cir.components_list[j].nodeleft;
        }
    }
    return returnnode;
}

// find which node is directly connected to the AC I source
string find_source_current_node(circuit cir){
    string returnnode;
    for(int j=0; j<cir.components_list.size(); j++) {
        if((cir.components_list[j].value == "SINE")&&(component_type(cir.components_list[j].name)=='I')){
            if(cir.components_list[j].noderight == "0"){
                returnnode=cir.components_list[j].nodeleft;
            }
            else{
                returnnode=cir.components_list[j].noderight;
            }
        }
    }
    return returnnode;
}

// find the type of the main AC source
string source_type(circuit cir) {
    string name;
    if(cir.voltages_src.frequency > 0){
            name = "V";
            return name;
    }
    else if(cir.currents_src.frequency > 0) {
        name = "I";
        return name;
    }
    else {
        assert(0);
    }
}

vector<float> set_time(circuit cir) {
    vector<float> time;
    float t = cir.simulation.start;
    while(t <= cir.simulation.stop) {
        if(t >= cir.simulation.save_data) {
            time.push_back(t);
        }
        t += cir.simulation.max_step;
    }
    return time;
}
/*
float current_betw_consec_nodes(component comp, circuit cir, circuitmat cmat, string nodeleft, string noderight) {
    vector<vector<component>> veccompbetweennode;
    //if capacitor is series with other capacitors, its a special case in which we have to add the capacitors together
    vector<component> leftcompbetweennode;
    for(int k=0; k< cir.components_list.size(); k++){
        if((cir.components_list[k].name != comp.name) && (cir.components_list[k].noderight ==  nodeleft || cir.components_list[k].nodeleft ==  nodeleft)){
        leftcompbetweennode.push_back(cir.components_list[k]);
        }
    }
    veccompbetweennode.push_back(leftcompbetweennode);

    vector<component> rightcompbetweennode;
    for(int k=0; k< cir.components_list.size(); k++){
        if((cir.components_list[k].name != comp.name) && (cir.components_list[k].noderight ==  noderight || cir.components_list[k].nodeleft ==  noderight)){
        rightcompbetweennode.push_back(cir.components_list[k]);
        }
    }
    veccompbetweennode.push_back(rightcompbetweennode);
    for(int w = 0; w < 2; w++){
        float sumcurrent=0;
        int count = 0;
        int vcount = 0;
        int p = 1;
        while(1){
            for(int t = 0; t< veccompbetweennode[w].size(); t++){
                float current=0;
                if(component_type(veccompbetweennode[w][t].name) == 'V'){
                    // count system to make sure this voltage is not alone and is actually parallel
                    count = count + 1;
                    vcount = 1;
                    current = 0; //parallel -> so probably no current inside?
                }
                else if(component_type(veccompbetweennode[w][t].name) == 'I'){ //current direction..?
                    count = count + 1;
                    current = veccompbetweennode[w][t].floatvalue;
                }
                else if(component_type(veccompbetweennode[w][t].name) == 'R'){
                    float voltageright , voltageleft; // TO DO find both sides of sides of the resistor, probably from the matrix
                    count = count + 1;
                    vector<string> node_list = remove_gnd_node(cir);
                    for(unsigned j=0; j<node_list.size(); ++j) {
                        if(node_list[j] == veccompbetweennode[w][t].nodeleft) {
                            voltageleft = cmat.vmat(j,0);
                        }
                        if(node_list[j] == veccompbetweennode[w][t].noderight) {
                            voltageright = cmat.vmat(j,0);
                        }
                    }
                    current = (voltageleft-voltageright)/veccompbetweennode[w][t].floatvalue;
                }
                else{
                    ;
                }
                if(vcount == 1 && count == 1){
                    break;
                }
                sumcurrent = sumcurrent + current;
                return sumcurrent;
            }
        }
    }
}
*/
/*float current_betw_consec_nodes(circuit cir, circuitmat cmat, string nodeleft, string noderight) {
    vector<component> resistors = find_component(cir, nodeleft, noderight);
    float resistance = 0;
    float one_float = 1;
    for(unsigned i=0; i<resistors.size(); ++i) {
        resistance = resistance + one_float/resistors[i].floatvalue;
    }
    resistance = one_float/resistance;
    float v_at_right, v_at_left;
    vector<string> node_list = remove_gnd_node(cir);
    for(unsigned j=0; j<node_list.size(); ++j) {
        if(node_list[j] == nodeleft) {
            v_at_left = cmat.vmat.at(j,0);
            cerr << "V_at_left: " << v_at_left << endl;
        }
        if(node_list[j] == noderight) {
            v_at_right = cmat.vmat.at(j,0);
            cerr << "V_at_right: " << v_at_right << endl;
        }
    }
    if(nodeleft == "0") {
        v_at_left = 0;
    }
    if(noderight == "0") {
        v_at_right = 0;
    }
    float v_across_comp = v_at_left - v_at_right;
    if(v_across_comp == 0) {
        return 0;
    }
    float current = v_across_comp / resistance;
    return current;
}*/

float voltage_across_inductor(circuit cir, circuitmat cmat, string nodeleft, string noderight) {
    float v_at_right, v_at_left;
    vector<string> node_list = remove_gnd_node(cir);
    for(unsigned j=0; j<node_list.size(); ++j) {
        if(node_list[j] == nodeleft) {
            v_at_left = cmat.vmat.at(j,0);
            //cerr << "V_at_left: " << v_at_left << endl;
        }
        if(node_list[j] == noderight) {
            v_at_right = cmat.vmat.at(j,0);
            //cerr << "V_at_right: " << v_at_right << endl;
        }
    }
    if(nodeleft == "0") {
        v_at_left = 0;
    }
    if(noderight == "0") {
        v_at_right = 0;
    }
    float v_across_inductor = v_at_left - v_at_right;
    return v_across_inductor;
}

bool diode_status(circuit cir, circuitmat cmat, component diode, float initial_v) {
    float v_prev = initial_v;
    float is = pow(10,-14);
    float vt = 0.025;
    float i_prev, g, v_next, i_next;
    int count1 = 0;
    int count2 = 0; 
    i_prev = is*exp((v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    g = is*exp(v_prev/vt)/vt;
    v_next = -i_prev/g + v_prev;
    
    i_next = is*exp((v_next/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    if (i_next < 0.01)
    {
        if (abs(v_next)<0.35)
        {
            return true;
        }

        else if (abs(v_next)>0.35)
        {
            return false;
        }
    }

    diode_status(cir, cmat, diode, v_next);


/*    while((abs(v_next - 0) > 0.1) && count1 < 100) {
        i_prev = is*(exp(v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
        g = is*exp(v_prev/vt)/vt;
        v_next = -i_prev/g + v_prev;
        v_prev = v_next;
        count1++;
        cerr << "first loop: " << v_next << endl;
    }
    cerr << "count: " << count1;
    v_prev = initial_v;
    while((abs(v_next - 0.7) > 0.1) && count2 < 100 && count1 < 100) {
        i_prev = is*(exp(v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
        cerr << "i prev: " << i_prev << endl;
        g = is*exp(v_prev/vt)/vt;
        cerr << "g: " << g << endl;
        v_next = -i_prev/g + v_prev;
        v_prev = v_next;
        count2++;
        cerr << "second loop: " << v_next << endl;
    }
    if(v_next < 0.35) {
        return false;
    }
    else {
        return true;
    } */
}

float diode_status_f(circuit cir, circuitmat cmat, component diode, float initial_v) {
    float v_prev = initial_v;
    float is = pow(10,-14);
    float vt = 0.025;
    float i_prev, g, v_next, i_next;
    int count1 = 0;
    int count2 = 0; 
    i_prev = is*exp((v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    g = is*exp(v_prev/vt)/vt;
    v_next = -i_prev/g + v_prev;

    i_next = is*exp((v_next/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    if (i_next < 0.01)
    {
        if (abs(v_next)<0.35)
        {
            return v_next;
        }

        else if (abs(v_next)>0.35)
        {
            return v_next;
        }
    }

    return diode_status(cir, cmat, diode, v_next);

    /*
    while((abs(v_next - 0) > 0.01) && count1 < 100) {
        i_prev = is*(exp(v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
        g = is*exp(v_prev/vt)/vt;
        v_next = -i_prev/g + v_prev;
        v_prev = v_next;
        count1++;
    }
    v_prev = initial_v;
    while((abs(v_next - 0.7) > 0.01) && count2 < 100 && count1 < 100) {
        i_prev = is*(exp(v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
        g = is*exp(v_prev/vt)/vt;
        v_next = -i_prev/g + v_prev;
        v_prev = v_next;
        count2++;
    }
    return v_next; */
}

bool diode_v(circuit cir, circuitmat cmat, component diode, float initial_v) {
    float v_prev = initial_v;
    float is = pow(10,-14);
    float vt = 0.025;
    float i_prev = is*exp((v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    float g = is*exp(v_prev/vt)/vt;
    float v_next = -i_prev/g + v_prev;
     if (abs(v_next - 0) < 0.1) {
        return false;
    }
    else if (abs(v_next - 0.7) < 0.1){
        return true;
    }
    else
    {
        diode_v(cir, cmat, diode, v_next);
    }
    
}

bool diode_in_circuit(circuit cir) {
    for(unsigned i=0; i<cir.components_list.size(); ++i) {
        if(cir.components_list[i].value == "D") {
            return true;
        }
    }
    return false;
}


float shockley(float v) {
    return pow(10,-12)*(exp(v/0.025)-1);
}

float shockley_derivative(float v) {
    return pow(10,-12)*(exp(v/0.025)-1)/0.025;
}
  
// Function to find the root 
float newtonRaphson(circuit cir, circuitmat cmat, component diode, float v) 
{ 
    float current = current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    double h = (shockley(v) + current) / (shockley_derivative(v) + current);
    while (abs(h) >= 0.01) 
    { 
        h = (shockley(v) + current) / (shockley_derivative(v) + current);  
        v = v - h; 
    } 
    return v;
} 

bool newtonRaphson_bool(circuit cir, circuitmat cmat, component diode, float v) 
{ 
    float current = current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    double h = (shockley(v) + current) / (shockley_derivative(v) + current);
    while (abs(h) >= 0.01) 
    { 
        h = (shockley(v) + current) / (shockley_derivative(v) + current);  
        v = v - h; 
    } 
    if(v<0.35) {
        return false;
    }
    else {
        return true;
    }
} 
  /*
void diode_state(circuit cir, circuitmat cmat, component diode, float v) {
    while(abs(v-))

    
    float v_prev = initial_v;
    float is = pow(10,-14);
    float vt = 0.025;
    float i_prev = is*exp((v_prev/vt) - 1) + current_betw_consec_nodes(diode, cir, cmat, diode.nodeleft, diode.noderight);
    float g = is*exp(v_prev/vt)/vt;
    float v_next = -g*i_prev + v_prev;
    if (abs(v_next - 0) < 0.1) {
        return 0;
    }
    else if (abs(v_next - 0.7) < 0.1){
        return 1;
    }
    else
    {
        diode_status(cir, cmat, diode, v_next);
    }
}*/

float current_betw_consec_nodes(component comp, circuit cir, circuitmat cmat, string nodeleft, string noderight) {
    vector<vector<component>> veccompbetweennode;
    //if capacitor is series with other capacitors, its a special case in which we have to add the capacitors together
    vector<component> leftcompbetweennode;
    for(int k=0; k< cir.components_list.size(); k++){
        if((cir.components_list[k].name != comp.name) && (cir.components_list[k].noderight ==  nodeleft || cir.components_list[k].nodeleft ==  nodeleft)  && !(cir.components_list[k].noderight ==  noderight || cir.components_list[k].nodeleft ==  noderight)){
        leftcompbetweennode.push_back(cir.components_list[k]);
        }
    }
    veccompbetweennode.push_back(leftcompbetweennode);

    vector<component> rightcompbetweennode;
    for(int k=0; k< cir.components_list.size(); k++){
        if((cir.components_list[k].name != comp.name) && (cir.components_list[k].noderight ==  noderight || cir.components_list[k].nodeleft ==  noderight) && !(cir.components_list[k].noderight ==  nodeleft || cir.components_list[k].nodeleft ==  nodeleft)){
        rightcompbetweennode.push_back(cir.components_list[k]);
        }
    }
    veccompbetweennode.push_back(rightcompbetweennode);

    vector<component> parallelcomps;
    for(int k=0; k< cir.components_list.size(); k++){
        if((cir.components_list[k].name != comp.name) && (cir.components_list[k].noderight ==  noderight || cir.components_list[k].nodeleft ==  noderight) && (cir.components_list[k].noderight ==  nodeleft || cir.components_list[k].nodeleft ==  nodeleft)){
        parallelcomps.push_back(cir.components_list[k]);
        }
    }
    veccompbetweennode.push_back(parallelcomps);

    float allincurrent;
    float sumcurrent=0;
for(int w = 0; w < 2; w++){
    float sumcurrent=0;
    int count = 0;
    int vcount = 0;
        for(int t = 0; t< veccompbetweennode[w].size(); t++){
            float current=0;
            if(component_type(veccompbetweennode[w][t].name) == 'V'){
                // count system to make sure this voltage is not alone and is actually parallel
                count = count + 1;
                vcount = 1;
                current = 0; //parallel -> so probably no current inside?
            }
            else if(component_type(veccompbetweennode[w][t].name) == 'I'){ //current direction..?
                count = count + 1;
                current = veccompbetweennode[w][t].floatvalue;
            }
            else if(component_type(veccompbetweennode[w][t].name) == 'R'){
                float voltageright , voltageleft; // TO DO find both sides of sides of the resistor, probably from the matrix
                count = count + 1;
                vector<string> node_list = remove_gnd_node(cir);
                for(unsigned j=0; j<node_list.size(); ++j) {
                    if(node_list[j] == veccompbetweennode[w][t].nodeleft) {
                        voltageleft = cmat.vmat(j,0);
                    }
                    if(node_list[j] == veccompbetweennode[w][t].noderight) {
                        voltageright = cmat.vmat(j,0);
                    }
                }
                current = (voltageleft-voltageright)/veccompbetweennode[w][t].floatvalue;
            }
            else{
                ;
            }
            sumcurrent = sumcurrent + current;
        }
    if(!(count == 1 && vcount ==1)){
        allincurrent = sumcurrent;
    }
}

if(!parallelcomps.size()==0 && component_type(comp.name) == 'V'){
     for(int k=0; k<parallelcomps.size(); k++){
        if(component_type(parallelcomps[k].name) == 'I'){
            allincurrent = allincurrent - parallelcomps[k].floatvalue;
        }
        if(component_type(parallelcomps[k].name) == 'V'){
            allincurrent = allincurrent;   
        }
        if(component_type(parallelcomps[k].name) == 'R'){\
            float voltager = 0;
            float voltagel = 0;
            float currents = 0;
            vector<string> node_list = remove_gnd_node(cir);
                for(unsigned j=0; j<node_list.size(); ++j) {
                    if(node_list[j] == parallelcomps[k].nodeleft) {
                        voltagel = cmat.vmat(j,0);
                    }
                    if(node_list[j] == parallelcomps[k].noderight) {
                        voltager = cmat.vmat(j,0);
                    }
                }
            currents = (voltagel-voltager)/parallelcomps[k].floatvalue;
            allincurrent = allincurrent - currents;
        }
    }
    return allincurrent;
}
else{
    return allincurrent;   
}
}