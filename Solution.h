#ifndef SOLUTION_H
#define SOLUTION_H

class Solution {
	
	float fo1,fo2; // stores the result of the objective functions for efficiency
	std::vector<Route> routes;
    

public:

	Solution(void);

    void quality();         //Calcula alterando el valor de fo1 y fo2
	bool check_feasability();
    
};
#endif
