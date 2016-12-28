#ifndef SNAKE_TEST_H
#define SNAKE_TEST_H

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



double snake_test_fn(Network * net, int * cycles);
int snake_test_routine(int winnerid, char * filename);
















#endif