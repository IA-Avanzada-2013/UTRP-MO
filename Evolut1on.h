#include "common.h"
#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "BusStop.h"
#include "Route.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"


class Evolut1on
{
public:
	Evolut1on(Problem p);
	~Evolut1on();

	Problem p;
	Solution route_set;
	int best_route_so_far_o1, best_route_so_far_o2;
	void generate_feasible_route_set();
	bool find_node(std::vector<BusStop>actual, int idx);
	void get_distinct_neighbour_bus_stops(int previous_node,std::vector<BusStop> actual,std::vector<BusStop> left, std::vector<BusStop> *out);
	bool is_neighbour(BusStop a, int b);
	void make_small_change();
	bool check_duplicates();
};
/*
Generate initial population of feasible route sets.
Calculate passenger and operator costs for each route set.
Record the best-route-set-so-far for both objectives.
repeat
for each route set in the population
	Apply the Make-Small-Change procedure to produce a
	feasible offspring
	if offspring is a duplicate
		then delete offspring
	elseif offspring improves on either best-so-far
		then offspring replaces parent and best-so-far updated
	elseif offspring dominates parent
		then offspring replaces parent
	elseif offspring and parent are mutually non-dominated
		then find an individual in the population that is domi-
		nated by the offspring and replace it with the offspring.
	endif
endfor
until the stopping condition is satisfied
print all non-dominated solutions in the final population.
*/