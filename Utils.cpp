#include "Utils.h"

void obtainReferencePoints(Problem *pbm, double *ref){
	//Initialize
	ref[0] = 0.0;
	ref[1] = 0.0;
	//std::cout<<"Nombre:"<<pbm->get_name()<<std::endl;
	if ((pbm->get_name()).compare("Mandl")== 0){
		ref[0] = 16; //15.1304;
		ref[1] = 225; //224;
	}
	else if ((pbm->get_name()).compare("Mumford0")== 0){
		ref[0] = 33; //32.3961;
		ref[1] = 760; //759;
	}
	else if ((pbm->get_name()).compare("Mumford1")== 0){
		ref[0] = 35; //34.688;
		ref[1] = 2039; //2038;
	}
	else if ((pbm->get_name()).compare("Mumford2")== 0){
		ref[0] = 37; //36.5416;
		ref[1] = 5633; //5632;
	}
	else if ((pbm->get_name()).compare("Mumford3")== 0){
		ref[0] = 37; //36.9159;
		ref[1] = 6666; //6665;
	}
	else{
	  std::cout<<"El problema no existe!";
	  return;
	}
	//std::cout<<"Problem: "<<pbm->get_name()<<": "<<ref[0]<<", "<<ref[1]<<std::endl;
	return;
}

double hv(SolutionSet *pop, Problem *pbm){
        double *data, *ref;
	double value;
	int i, k, j;
	
	int npts=pop->solutions.size();
	int nobj=2;
	
	data = (double *)malloc(npts*nobj*sizeof(double));
	ref = (double *)malloc(nobj*sizeof(double));
	
	j = 0;
	for (std::vector<Solution>::iterator it =  pop->solutions.begin(); it!= pop->solutions.end(); ++it)
	{
	  data[j] = it->fo1; j++;
	  data[j] = it->fo2; j++;
	}
	
	//std::cout<<std::endl;
	//for (i = 0; i < npts*nobj; i++)
	//  std::cout<<"\t"<<data[i]<<std::endl;
	//std::cout<<std::endl;
	
	obtainReferencePoints(pbm, ref);
	
	value = fpli_hv(data, nobj, npts, ref);

	free(data);
	return value;
}