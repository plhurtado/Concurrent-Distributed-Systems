#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define ETIQUETAFILO 0
#define ETIQUETATENE 1
#define ETIQUETATENElib 2
#define ETIQUETAsentarse 3
#define ETIQUETAlevantarse 4
#define CAMARERO 10

using namespace std;

void Camarero (void){
	int filo, value, etiqueta, ocupados = 0;

	MPI_Status status;

	while (true){
		
		if (ocupados == 0){
			MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, ETIQUETAsentarse, MPI_COMM_WORLD, &status);
			filo = status.MPI_SOURCE;
			cout << "Camarero recibe peticion de sentarse de " << filo << endl;
			ocupados++;
			//cout << "En este momentos hay                              " << ocupados<<endl;
		}
		else if (ocupados == 4){
			MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, ETIQUETAlevantarse, MPI_COMM_WORLD, &status);
			filo = status.MPI_SOURCE;
			cout << "Camarero recibe peticion de levantarse de " << filo << endl;
			ocupados--;
			//cout << "En este momentos hay                              " << ocupados<<endl;
		}
		else{
			MPI_Recv (&value, 1, MPI_INT, MPI_ANY_SOURCE,  MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			etiqueta = status.MPI_TAG;

			if (etiqueta == ETIQUETAsentarse){
				filo = status.MPI_SOURCE;
				cout << "Camarero recibe peticion de sentarse de " << filo << endl;
				ocupados++;
				//cout << "En este momentos hay                              " << ocupados<<endl;
			}
			else{
				filo = status.MPI_SOURCE;
				cout << "Camarero recibe peticion de levantarse de " << filo << endl;
				ocupados--;
				
			}
		}
		cout << "En este momentos hay                              " << ocupados<<endl;
	}	

}

void Filosofo(int id, int nprocesos){
	//int value;
	int peticion;
	int izq = (id+1)%(nprocesos-1);
	int der = (id-1+(nprocesos-1))%(nprocesos-1);

	//MPI_Status status;

	while (true){
		//Pensamos
		//cout << "Filosofo " << id << " PENSANDO " << endl;
		sleep ((rand()%3)+1);

		//Solicita sentarse
		MPI_Ssend (&peticion, 1, MPI_INT, CAMARERO, ETIQUETAsentarse, MPI_COMM_WORLD);
		cout << "Filosofo " << id << " solicita sentarse " << endl;
		//MPI_Recv (&value, 1, MPI_INT, CAMARERO, ETIQUETAsentarse, MPI_COMM_WORLD, &status);

		//Solicita tenedor izquierda
		cout << "Filosofo " << id << " solicita tenedor izq. " << izq << endl;
		MPI_Ssend (&peticion, 1, MPI_INT, izq, ETIQUETAFILO, MPI_COMM_WORLD);
		//MPI_Recv(&value, 1, MPI_INT, izq, ETIQUETATENE, MPI_COMM_WORLD, &status);

		//Solicita tenedor derecha
		cout << "Filosofo " << id << " solicita tenedor der. " << der << endl;
		MPI_Ssend (&peticion, 1, MPI_INT, der, ETIQUETAFILO, MPI_COMM_WORLD);
		//MPI_Recv(&value, 1, MPI_INT, der, ETIQUETATENE, MPI_COMM_WORLD, &status);
		
		//Comemos
		cout << "Filosofo " << id << " COMIENDO " << endl;
		sleep ((rand()%3)+1);

		//Soltamos tenedor izquierdo
		cout << "Filosofo " << id << " suelta tenedor izq. " << izq << endl;
		MPI_Ssend(&peticion, 1, MPI_INT, izq, ETIQUETATENElib, MPI_COMM_WORLD);

		//Soltamos tenedor derecho
		cout << "Filosofo " << id << " suelta tenedor der. " << der << endl;
		MPI_Ssend(&peticion, 1, MPI_INT, der, ETIQUETATENElib, MPI_COMM_WORLD);

		//Solicita levantarse
		
		MPI_Ssend (&peticion, 1, MPI_INT, CAMARERO, ETIQUETAlevantarse, MPI_COMM_WORLD);
		cout << "Filosofo " << id << " solicita levantarse " << endl;
		//MPI_Recv (&value, 1, MPI_INT, CAMARERO, ETIQUETAlevantarse, MPI_COMM_WORLD, &status);
	}	
}

void Tenedor (int id, int nprocesos){
	int buf, Filo;

	MPI_Status status;

	while (true){
		MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, ETIQUETAFILO, MPI_COMM_WORLD, &status);
		Filo = status.MPI_SOURCE;
		cout << "Tenedor " << id << " recibe peticion de " << Filo << endl;
		//MPI_Send(&buf, 1, MPI_INT, Filo, ETIQUETATENE, MPI_COMM_WORLD);
		MPI_Recv(&buf, 1, MPI_INT, Filo, ETIQUETATENElib, MPI_COMM_WORLD, &status);
		cout << "Tenedor " << id << " recibe liberacion de " << Filo << endl;
	}
}

int main (int argc, char** argv){
	int rank, size;
	srand (time(0));
	MPI_Init (&argc,&argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	if (size != 11){
		if (rank==0)
			cout << "El numero de procesos debe ser 11" << endl;
		MPI_Finalize(); 
		return 0;
	}
	if (rank == 10)			Camarero();
	else if (rank%2==0) 	Filosofo(rank,size);
	else 					Tenedor(rank,size);

	MPI_Finalize();
	
	return 0;
}