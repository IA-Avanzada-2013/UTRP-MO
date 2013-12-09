class Problem {

    public:
      BusStop bus_stops[SIZE];
  //coordinate_t coordinate_array[SIZE];

            int demand[SIZE][SIZE];
            int travel_time[SIZE][SIZE];//-1 if infinite?
        Problem();
        ~Problem();
        int routes_min, routes_max;
       
        
        int get_demand(); // calculates the total demand adding all demands
        
        
    private:

}