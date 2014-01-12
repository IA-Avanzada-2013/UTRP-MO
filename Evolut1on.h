#include "common.h"
#include "BusStop.h"
#include "Route.h"
#include "SolutionSet.h"


class Evolut1on
{
public:
	Evolut1on(Problem p, int seed,std::vector<RouteInfo> routes_info); 
	~Evolut1on();

	Problem p;
	SolutionSet result;
	int seed,route_types,total_routes,route_min_length,route_max_length,route_type;
	int** routes_info;

	int best_route_so_far_o1, best_route_so_far_o2;
	Solution *generate_feasible_route_set(std::vector<RouteInfo> routes_info);
	Route *generate_route(int tmp_route_size,std::vector<BusStop> used_nodes, std::vector<BusStop>&left_bus_stops,int tmp_route_type);
	bool find_node(std::vector<BusStop>actual, int idx);
	std::vector<BusStop> get_neighbour_bus_stops(int previous_node,std::vector<BusStop> route_in_creation,std::vector<BusStop> bus_stops_left);
	bool is_neighbour(BusStop a, int b);
	bool repair_routeset(Solution *unfeasible_routeset);
	void make_small_change();
	bool check_duplicates();
	int nice_neighbour(std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops);
	std::vector<BusStop> get_specific_nodes(Solution *routeset,bool status);
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