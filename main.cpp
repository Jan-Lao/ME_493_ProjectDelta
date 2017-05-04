//
//  main.cpp
//  Project Delta Draft 1
//
//  Created by Jan Lao on 4/28/17.
//  Copyright © 2017 University Nevada, Reno. All rights reserved.
//

#include <iostream>
#include <assert.h>
#include <random>
#include <vector>
#include <math.h>
#include <fstream>
#include "LY_NN.h"

#define JLRAND (double)rand()/RAND_MAX
#define PI 3.14159

using namespace std;

class Lilboat{
public:
    double startxt;
    double startyt;
    double xt;
    double yt;
    double xtplus1;
    double ytplus1;
    double theta;
    double omega;
    double v;
    double dt;
    double T;
    double u;
    
    void shipinit();
    void shiptestinit();
    void simphys();
    bool ingoal(double targetx1, double targety1, double targety2);
    bool outbounds();
};

class Map{
public:
    double xdim;
    double ydim;
    double targetx1;
    double targety1;
    double targetx2;
    double targety2;
    
    void mapinit();
};

class Policy{
public:
    double fitness;
    int ts;
    
    void policyinit(int num_of_weights);
    void mutate();
    void policyeval(Lilboat Boat);
    vector<double> weights;
};

/////===============================BOAT FUNCTIONS========================================
void Lilboat::shipinit(){
    startxt = rand()%1000; //placeholders for every episode             //HR_4
    startyt = rand()%1000;
    xt = startxt;
    yt = startyt;
    theta = (rand()%360)*PI/180;
    omega = rand()%10;
    v = 3.0;
    dt = 0.2;
    T = 5;
    u = 0;
    
    cout << "CREATING BOAT:........Xpos: " << startxt << "\t" << "Ypos: " << startyt << endl << endl;
}

void Lilboat::shiptestinit(){ //MR_1
    startxt = 945; //placeship directly to the left of the goal
    startyt = 800; //Setting the location of the boat
    xt = startxt;
    yt = startyt;
    theta = 0; //HR_3 -- Setting orientation of boat
    omega = 0;
    v = 3.0;
    dt = 0.2;
    T = 5;
    u = 0;
    
    cout << "SETTING THE BOAT DIRECTLY LEFT OF THE GOAL" << endl;
    cout << "CREATING BOAT:........Xpos: " << startxt << "\t" << "Ypos: " << startyt << endl << endl;
    
}

void Lilboat::simphys(){
    xtplus1 = xt + v*sin(theta)*dt; //LR_8
    ytplus1 = yt + v*cos(theta)*dt;
    theta = theta + omega*dt;
    omega = omega + (u-omega)*dt/T;
    
    xt = xtplus1;
    yt = ytplus1;
}

bool Lilboat::ingoal(double targetx1, double targety1, double targety2){ //MR_2
    //Thanks to Sierra Gonzales:
    ///Calculate line of boat from timestep to previous timestep...To check if it crossed
    double m; //slope of boat
    double b; //y-int of boat
    double y; //equation of the boat line
    m = (xtplus1-xt)/(ytplus1-yt);
    b = ytplus1-m*xtplus1;
    y = m*950/*targetx1*/+b;
    
    if(y>targety1 && y<targety2){ //Thanks to Honi A.
        return true;
    }
    return false;
}

bool Lilboat::outbounds(){
    if(xt<0 || xt>1000 || yt<0 || yt>1000){ //Thanks to Honi A.
        return true;
    }
    return false;
}

/////===============================MAP FUNCTIONS========================================
void Map::mapinit(){
    xdim = 1000;
    ydim = 1000;
    targetx1 = 950;
    targety1 = 950;
    targetx2 = targetx1; //The goal is a vertical line on the top right corner
    targety2 = 750;
    
    cout << "CREATING WORLD:.......Xdim: " << xdim << "\t" << "Ydim: " << ydim << endl;
    cout << "CREATING GOAL:........Xtarget1: " << targetx1 << "\t" << "Ytarget1: " << targety1 << endl;
    cout << "CREATING GOAL:........Xtarget2: " << targetx2 << "\t" << "Ytarget2: " << targety2 << endl << endl;
}

/////===============================POLICY FUNCTIONS========================================
void Policy::policyinit(int num_of_weights){
    vector<double> q;
    for(int i=0; i<num_of_weights; i++){
        q.push_back(i);
    }
    weights = q;
}

/////===============================EVOLUTONARY ALGORITHM========================================
////------------------------------------EA INITIALIZE-------------------------------------------
/*vector<Policy> EA_init(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    ..................NOTE: THIS IS ALREADY IN MAIN PLEASE GO ON.......................
    return population;
}; */

////------------------------------------EA REPLICATE-------------------------------------------
void Policy::mutate(){
    //what should I do to mutate a policy? add a randnumb gen and subtract randnumb gen
    for(int i = 0; i<weights.size(); i++){
        if(rand()%2==1){ //Thanks Dr. Y!
            //Can't mutate every weight right?
            weights.at(i) = weights.at(i) + JLRAND - JLRAND; //LR_5
        }
    }
}

vector<Policy> EA_replicate(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
    assert(population.size() == popsize/2);
    
    while(population.size()<popsize){
        int spot = rand()%population.size();
        Policy A;
        A = population.at(spot);
        A.mutate();
        population.push_back(A);
    }
    
    return population;
};

////------------------------------------EA EVALUATE-------------------------------------------
void Policy::policyeval(Lilboat LilYachty){ //MR_2
    //how do i evaluate... I could go based on my output (u or distance)
    //must assign a physical equation to = fitness
    
    double delta_x;
    double delta_y;
    double tot_distance;
    delta_x = 950 - LilYachty.xt;
    delta_y = 850 - LilYachty.yt;
    tot_distance = sqrt((delta_x*delta_x)+(delta_y*delta_y));
    
    fitness = tot_distance; //MR_4
}

vector<Policy> EA_evaluate(vector<Policy> pop, int popsize, Lilboat LilYachty){
    vector<Policy> population;
    population = pop;
    
    //Assign fitness
    for(int i=0; i<population.size(); i++){
        population.at(i).policyeval(LilYachty); //MR_4
    }
    
    return population;
};

////------------------------------------EA DOWNSELECT-------------------------------------------
vector<Policy> EA_downselect(vector<Policy> pop, int popsize){ //Straight from Project Gamma
    vector<Policy> population;
    
    assert(population.size() == 0);
    //cout << population.size() << endl; //debugging
    
    //BINARY TOURNAMENT COMMENCED
    while(population.size() != popsize/2){ //MR_4
        int spot1 = rand()%popsize;
        int spot2 = rand()%popsize;
        while(spot2 == spot1){
            spot2 = rand()%popsize;
        }
        assert(spot1!=spot2);
        double fit1 = pop.at(spot1).fitness;
        double fit2 = pop.at(spot2).fitness;
        
        if(fit1<fit2){
            //fit 1 wins -> put into population vector
            Policy A1 = pop.at(spot1);
            population.push_back(A1);
        }
        else if(fit2<=fit1){
            //fit 2 wins -> put into population vector
            Policy A2 = pop.at(spot2);
            population.push_back(A2);
        }
    }
    assert(population.size() == popsize/2); //
    //cout << population.size() << endl; //debugging
    return population;
};

/////===============================MAIN FUNCTION========================================
int main(){
    srand(time(NULL));
    
    int maxgeneration = 300;
    int popsize = 100;
    cout << "Welcome to NonLinear Evolutionary Control!" << endl;
    
    ////--------------------------INITIALIZE PHYSICAL OBJECTS-----------------------------
    Map PacificOcean; //LR_2
    PacificOcean.mapinit();
    
    int w;
    cout<< "Are you testing the program?........ 1(Y).......2(N)" << endl;
    cin >> w;
    
    Lilboat LilYachty; //LR_1
    if(w==2){
        LilYachty.shipinit();
    }
    else{
        LilYachty.shiptestinit(); //HR_1 AND HR_3
    }
        
    neural_network NN; //LR_4
    NN.setup(3/*input*/, 5/*hiddenlayer*/, 1/*output*/);
    
    cout << "SETTING UP NEURAL NETWORK:................DONE" << endl;
    cout << "-///////////////////////////////////////////////////////////////////////////-" << endl << endl;
    
    ////////////////////////////////////PROJECT DELTA/////////////////////////////////////
    ////---------------------------------NEURAL NETWORK-------------------------------------
    //TREAT AS BLACK BOX -- 3 inputs(show lower and upper bound) -- 5 hidden layers --- 1 output
    NN.set_in_min_max(0,1000); //xdim       //LR_6
    NN.set_in_min_max(0,1000); //ydim
    NN.set_in_min_max(0,2*PI); //theta
    NN.set_out_min_max(-15,15); //u         //LR_7
    
    ////-----------------------------EVOLUTIONARY ALGORITHM--------------------------------
    vector<Policy> pop;
    
    ///Initializing the policies to initialize the population--->
    ///pop = EA_init(popsize); IS BELOW~~~~~~~~~~~~~~~~~~
    
    int num_weights = NN.get_number_of_weights();
    
    for(int i=0; i<popsize; i++){
        Policy pol;
        pol.policyinit(num_weights);
        pop.push_back(pol);
    }
    
    //cout << "EA initialized" << endl;
    
    ofstream fout("/Users/Jan/Desktop/ProjectDelta_Boat_Locations.txt", fstream::trunc);
    
    for(int i=0; i<maxgeneration; i++){
        for(int s=0; s<popsize; s++){
            
            // DEBUGGING HERE
            //cout << popsize << endl;
             
            /*for(int j=0; j<popsize; j++){
                cout << pop.at(s).weights.at(j) << "..." << endl; //vector error
            }*/
            //cout << pop.at(s).weights.size() << endl;
            //cout << pop.size() << endl;
            assert(pop.size() == popsize);
            
            NN.set_weights(pop.at(s).weights, true); //setting the weights
            
            int timestep = 0; //initializing timestep first
    
            while(timestep < 10000){
                vector<double> state;
                state.push_back(LilYachty.xt); //location much like gridworld //MR_3
                //cout << state.at(s) << endl;
                state.push_back(LilYachty.yt); //location much like gridworld //MR_3
                state.push_back(LilYachty.theta); //orientation of travel //MR_3
            
                NN.set_vector_input(state);
                NN.execute(); //NN computes what u should be
                LilYachty.u = NN.get_output(0);
                LilYachty.simphys(); //simulate timestep
                
                pop.at(s).ts = timestep;
                
                /*if (timestep > 1000){
                    break; //Thank you Dr. LY!...If this gets too long end the program and rethink solutions
                }*/
                
                if (LilYachty.outbounds() ){
                    cout << "The boat is out of bounds" << endl;
                    break; //If ship gets out of bound stop the program       //LR_3
                }
                
                if (LilYachty.ingoal(PacificOcean.targetx1, PacificOcean.targety1, PacificOcean.targety2)){
                    cout << "The boat made it through the goal!" << endl;
                    break;
                    
                }
                
                pop = EA_evaluate(pop, popsize, LilYachty);
                //cout << "EA evaluated..." << endl;
                
                timestep++;
            }
            
            cout << "Xpos:  " << LilYachty.xt << " \t" << "Ypos:  " << LilYachty.yt <<  " \t" << "Theta:  " << LilYachty.theta << endl;
            
            fout << LilYachty.xt << "      " << LilYachty.yt << endl;
        }
        pop = EA_downselect(pop, popsize);
        //cout << "EA downselected..." << endl;
        
        pop = EA_replicate(pop, popsize);
        //cout << "EA replicateded..." << endl;
        
        fout.close();
    }
    
    for(int i=0; i<popsize; i++){
        cout << i << " /t" << pop.at(i).fitness << " /t" << pop.at(i).ts << endl; //HR_2
    }
    return 0;
}
