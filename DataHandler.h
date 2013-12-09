class DataHandler
{
  public:
      DataHandler(void);
      ~DataHandler(void);
      
      error_t read_coords(char *filename, struct Ivan *result);
      error_t read_demand(char *filename, struct Ivan *result);
      error_t read_travel_times(char *filename, struct Ivan *result);
      error_t read_route_type(char *filename, struct Ivan *result);
      
  private:
}