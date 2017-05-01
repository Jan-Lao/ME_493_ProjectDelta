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
#include "LY_NN.h"

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
    void simphys();
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
    
    void policyinit();
    vector<double> weights;
};

/////===============================BOAT FUNCTIONS========================================
void Lilboat::shipinit(){
    startxt = rand()%100; //placeholders for every episode
    startyt = rand()%100;
    xt = startxt;
    yt = startyt;
    theta = rand()%360*3.14159/260;
    omega = rand()%10;
    v = 3.0;
    dt = 0.2;
    T = 5;
    u = 0;
}

void Lilboat::simphys(){
    xtplus1 = xt + v*sin(theta)*dt;
    ytplus1 = yt + v*cos(theta)*dt;
    theta = theta + omega*dt;
    omega = omega + (u-omega)*dt/T;
    
    //Thanks to Sierra Gonzales, the worst person ever:
    ///Calculate line of boat from timestep to previous timestep...To check if it crossed
    double m; //slope of boat
    double b; //y-int of boat
    double y; //equation of the boat line
    m = (xtplus1-xt)/(ytplus1-yt);
    b = ytplus1-m*xtplus1;
    //y = m*targetx1+b; //??? talk to Sierra about this tho
}

/////===============================MAP FUNCTIONS========================================
void Map::mapinit(){
    //xdim = 100;
    //ydim = 100;
    targetx1 = 95;
    targety1 = 95;
    targetx2 = targetx1;
    targety2 = 75;
}

/////===============================POLICY FUNCTIONS========================================
void Policy::policyinit(){
    //I honestly don't know how to do this part, hold on
}

/////===============================EVOLUTONARY ALGORITHM========================================
vector<Policy> EA_init(){
    
    //not sure if i need this here... Further research is required
    
    return population;
};

vector<Policy> EA_replicate(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
    return population;
};

vector<Policy> EA_evaluate(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
    return population;
};

vector<Policy> EA_downselect(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
    return population;
};

/////===============================MAIN FUNCTION========================================
int main(){
    srand(time(NULL));
    
    int maxgeneration = 300;
    int popsize = 100;
    
    ////--------------------------INITIALIZE PHYSICAL OBJECTS-----------------------------
    Map PacificOcean;
    PacificOcean.mapinit();
    
    Lilboat LilYachty;
    LilYachty.shipinit();
    
    neural_network NN;
    NN.setup(3/*input*/, 5/*hiddenlayer*/, 1/*output*/);
    
    ////---------------------------------NEURAL NETWORK-------------------------------------
    NN.set_in_min_max(0,100); //xdim?
    NN.set_in_min_max(0,100); //ydim?
    //NN.set_in_min_max(something, something);
    NN.set_out_min_max(-15,15); //See video pls and instructions pls
    
    ////-----------------------------EVOLUTIONARY ALGORITHM--------------------------------
    vector<Policy> pop;
    
    ///Initializing the policies to initialize the population --->population = EA_init();
    for(int i=0; i<popsize; i++){
        Policy pol;
        pol.policyinit(); //still need to set up policy initializer
        pop.push_back(pol);
    }
    
    for(int i=0; i<maxgeneration; i++){
        //uuuuuuhhhh EA_init()????
        //????
        //Profit
        
        for(double sim=0; sim<popsize; sim++){
            NN.set_weights(pop.at(sim).weights, true); //setting the weights
            
            int timestep = 0; //initializing timestep first
            
            //while(){
        
            //}
        }
        
        
        pop = EA_replicate(pop, popsize);
        
        pop = EA_evaluate(pop, popsize);
        
        pop = EA_downselect(pop, popsize);
        
    }
    
    
    return 0;
}
