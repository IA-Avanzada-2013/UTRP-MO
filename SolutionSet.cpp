#include "SolutionSet.h"

SolutionSet::SolutionSet(void);

bool SolutionSet::print_solution_set(){
	std::string filename;
	filename = "Results/Pareto.txt";
	std::ofstream paret(filename.c_str());

	for (std::vector<Solution>::iterator it =  this.begin(); it!= this.end(); ++it)
	{
		paret << it->fo1;
		paret << " , ";
		paret << it->fo2;
		paret << std::endl;
    }
    paret.close();
};