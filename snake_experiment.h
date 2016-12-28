#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include "neat.h"
#include "network.h"
#include "population.h"
#include "organism.h"
#include "genome.h"
#include "species.h"

using namespace std;

using namespace NEAT;



Population *snake_test(int gens, int gen_id, char * filename, char * temp_name); 
bool snake_evaluate(Organism *org);
int snake_epoch(Population *pop,int generation,char *filename);
double snake_fitness(Network * net, double * snake_l);














#endif