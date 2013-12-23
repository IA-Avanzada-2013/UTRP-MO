#ifndef SOLUTION_SET_H
#define SOLUTION_SET_H

#include <vector>

#include "Solution.h"

class SolutionSet { 
  int id;
  std::vector <Solution> solutions;

public:
	SolutionSet(void){};
	bool print_solution_set();
};

#endif
