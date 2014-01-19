#ifndef OPCIONES_H
#define OPCIONES_H

class Opciones{
public:
	Opciones();
	
	//getters
	int get_popsize();
	float get_alpha();
	float get_beta();
	int get_clonsize();
	float get_probmutacion();
	float get_afinidad();
	int get_generaciones();
	float get_porcentajeclones();
	float get_porcentajereemplazo();
	int get_size();
	
	//setters
	void set_popsize(int);
	void set_alpha(float);
	void set_beta(float);
	void set_clonsize(int);
	void set_probmutacion(float);
	void set_afinidad(float);
	void set_generaciones(int);
	void set_porcentajeclones(float);
	void set_porcentajereemplazo(float);
	void set_size(int);
	
	Opciones& operator=(Opciones*);
	
private:
	int pop_size;
	float alpha;
	float beta;
	int clon_size;
	float prob_mutacion;
	float afinidad;
	int generaciones;
	float porcentaje_clones;
	float porcentaje_reemplazo;
	int size;
};


#endif
