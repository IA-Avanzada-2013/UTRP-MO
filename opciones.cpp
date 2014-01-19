#include "opciones.h"

Opciones::Opciones()
{
	
}

//getters
int Opciones::get_popsize()
{
	return this->pop_size;
}

float Opciones::get_alpha()
{
	return this->alpha;
}

float Opciones::get_beta()
{
	return this->beta;
}

int Opciones::get_clonsize()
{
	return this->clon_size;
}

float Opciones::get_probmutacion()
{
	return this->prob_mutacion;
}

float Opciones::get_afinidad()
{
	return this->afinidad;
}

int Opciones::get_generaciones()
{
	return this->generaciones;
}

float Opciones::get_porcentajeclones()
{
	return this->porcentaje_clones;
}

float Opciones::get_porcentajereemplazo()
{
	return this->porcentaje_reemplazo;
}

int Opciones::get_size()
{
	return this->size;	
}

//setters
void Opciones::set_popsize(int valor)
{
	this->pop_size = valor;
}

void Opciones::set_alpha(float valor)
{
	this->alpha = valor;
}

void Opciones::set_beta(float valor)
{
	this->beta = valor;
}

void Opciones::set_clonsize(int valor)
{
	this->clon_size = valor;
}

void Opciones::set_probmutacion(float valor)
{
	this->prob_mutacion = valor;
}

void Opciones::set_afinidad(float valor)
{
	this->afinidad = valor;
}

void Opciones::set_generaciones(int valor)
{
	this->generaciones = valor;	
}

void Opciones::set_porcentajeclones(float valor)
{
	this->porcentaje_clones = valor;
}

void Opciones::set_porcentajereemplazo(float valor)
{
	this->porcentaje_reemplazo = valor;
}

void Opciones::set_size(int valor)
{
	this->size = valor;
}

Opciones& Opciones::operator = (Opciones *opc)
{
	this->set_popsize(opc->get_popsize());
	this->set_alpha(opc->get_alpha());
	this->set_beta(opc->get_beta());
	this->set_clonsize(opc->get_clonsize());
	this->set_probmutacion(opc->get_probmutacion());
	this->set_afinidad(opc->get_afinidad());
	this->set_generaciones(opc->get_generaciones());
	this->set_porcentajeclones(opc->get_porcentajeclones());
	this->set_porcentajereemplazo(opc->get_porcentajereemplazo());
	this->set_size(opc->get_size());
}
