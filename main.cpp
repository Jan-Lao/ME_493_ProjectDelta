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
    bool ingoal(double targetx1, double targety1, double targety2);
    bool inbounds();
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
    
    void policyinit();
    void mutate();
    void policyeval();
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
}

bool Lilboat::ingoal(double targetx1, double targety1, double targety2){
    //Thanks to Sierra Gonzales, the worst person ever:
    ///Calculate line of boat from timestep to previous timestep...To check if it crossed
    double m; //slope of boat
    double b; //y-int of boat
    double y; //equation of the boat line
    m = (xtplus1-xt)/(ytplus1-yt);
    b = ytplus1-m*xtplus1;
    y = m*95/*targetx1*/+b;
    
    if(y>targety1 && y<targety2){ //Honi A.
        return true;
    }
    return false;
}

bool Lilboat::inbounds(){
    if(xt<0 || xt>100 || yt<0 || yt>100){ //Honi A.
        return true;
    }
    return false;
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
////------------------------------------EA INITIALIZE-------------------------------------------
vector<Policy> EA_init(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
    
    return population;
};

////------------------------------------EA REPLICATE-------------------------------------------
void Policy::mutate(){
    //what should I do to mutate a policy?
}

vector<Policy> EA_replicate(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
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
void Policy::policyeval(){
    //how do i evaluate... I could go based on my output (u or distance)
    //must assign a physical equation to =fitness
}

vector<Policy> EA_evaluate(vector<Policy> pop, int popsize){
    vector<Policy> population;
    population = pop;
    
    //Assign fitness
    for(int i=0; i<population.size(); i++){
        population.at(i).policyeval();
    }
    
    return population;
};

////------------------------------------EA DOWNSELECT-------------------------------------------
vector<Policy> EA_downselect(vector<Policy> pop, int popsize){ //Straight from Project Gamma
    vector<Policy> population;
    population = pop;
    
    assert(population.size() == 0);
    assert(population.size() == popsize);
    
    //cout << population.size() << endl; //debugging
    
    //BINARY TOURNAMENT COMMENCED
    while(population.size() < popsize/2){ //MR_4
        int spot1 = rand()%population.size();
        int spot2 = rand()%population.size();
        while(spot2 == spot1){
            spot2 = rand()%population.size();
        }
        assert(spot1!=spot2);
        double fit1 = population.at(spot1).fitness;
        double fit2 = population.at(spot2).fitness;
        
        if(fit1<fit2){
            //fit 1 wins -> put into population vector
            Policy A1 = population.at(spot1);
            population.push_back(A1);
        }
        else if(fit2<=fit1){
            //fit 2 wins -> put into population vector
            Policy A2 = population.at(spot2);
            population.push_back(A2);
        }
    }
    assert(population.size() == popsize/2);
    //cout << population.size() << endl; //debugging
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
    
    ///Initializing the policies to initialize the population--->population = EA_init();
    for(int i=0; i<popsize; i++){
        Policy pol;
        pol.policyinit(); //still need to set up policy initializer
        pop.push_back(pol);
    }
    
    for(int i=0; i<maxgeneration; i++){
        for(double sim=0; sim<popsize; sim++){
            NN.set_weights(pop.at(sim).weights, true); //setting the weights
            
            int timestep = 0; //initializing timestep first
            
            while(!LilYachty.ingoal(PacificOcean.targetx1, PacificOcean.targety1, PacificOcean.targety2) && !LilYachty.inbounds()){ //Thanks to Honi Ahmadian for this while-cond and bool funcs
                vector<double> state;
                state.push_back(LilYachty.xt); //location much like gridworld
                state.push_back(LilYachty.yt); //location much like gridworld
                state.push_back(LilYachty.theta); //orientation of travel
            
                NN.set_vector_input(state);
                NN.execute(); //NN computes what u should be
                LilYachty.u = NN.get_output(0);
                LilYachty.simphys(); //simulate timestep
            
                //population.at(sim).fitness = fabs();
                pop.at(sim).ts = timestep;
                
                timestep++;
                
                if (timestep > 1000){
                    break; //Thank you Dr. LY!
                }
            }
        }
        pop = EA_init(pop, popsize);
        
        pop = EA_replicate(pop, popsize);
        
        pop = EA_evaluate(pop, popsize);
        
        pop = EA_downselect(pop, popsize);
    }
    
    for(int i=0; i<popsize; i++){
        cout << i << " /t" << pop.at(i).fitness << " /t" << pop.at(i).ts << endl;
    }
    return 0;
}
