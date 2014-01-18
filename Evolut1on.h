#include "common.h"
#include "BusStop.h"
#include "Route.h"
#include "SolutionSet.h"


class Evolut1on
{
public:
	Evolut1on(Problem p, int seed, std::vector<RouteInfo> routes_info, int solset_size, float prob_mutation, float prob_crossover, int fxeval); 
	~Evolut1on(void);

	Problem p;
	SolutionSet result;
	int seed,route_types,total_routes,route_min_length,route_max_length,route_type,number_of_routes;
	int** routes_info;
	int solset_size, fxeval;
	float prob_mutation, prob_crossover;

	int best_routeset_so_far_o1, best_routeset_so_far_o2;
	Solution *generate_feasible_route_set(std::vector<RouteInfo> routes_info);
	Route *generate_route(int tmp_route_size,std::vector<Route> routes_so_far, int tmp_route_type);
	bool find_node(std::vector<BusStop>actual, int idx);
	std::vector<BusStop> get_neighbour_bus_stops(int previous_node,std::vector<BusStop> route_in_creation,std::vector<BusStop> bus_stops_left);
	bool is_neighbour(BusStop a, int b);
	bool repair_routeset(Solution *unfeasible_routeset);
	bool check_duplicate_routes(Solution *routeset, Route *route);
	int nice_neighbour(std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops);
	std::vector<BusStop> get_specific_nodes(Solution *routeset,bool status);
	void print_temp_route(std::vector<BusStop> ruta);
	bool find_route(std::vector<Route>actual, Route ruta);
	void seamo2(void);
	void set_best_routes(void);
	Solution *crossover(Solution P1, Solution P2);
	int nice_route(std::vector<Route> route_set,std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops, int route_type,std::vector<Route> actual_route_set);
	bool check_feasability2(Solution *sol);
	void set_fs(Solution *sol);
	int get_route_type(int i);
	void *mutate(Solution *offspring);
	std::vector<BusStop> refill_route(std::vector<BusStop> in, int diff);
	bool domination(Solution *offspring, Solution parent);
	bool check_duplicate_route_set(SolutionSet *solset, Solution *offspring);
	bool is_reached(int idx, std::vector<Route> routes);
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