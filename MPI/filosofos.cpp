#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define ETIQUETAFILO 0
#define ETIQUETATENE 1
#define ETIQUETATENElib 2

using namespace std;

void Filosofo(int id, int nprocesos){
	int peticion;
	int izq = (id+1)%nprocesos;
	int der = (id+nprocesos-1)%nprocesos;

	while (true){
		if (id!=0){
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

			//Pensamos
			cout << "Filosofo " << id << " PENSANDO " << endl;
			sleep ((rand()%3)+1);
		}	
		else{
			//Solicita tenedor derecho
			cout << "Filosofo " << id << " solicita tenedor der. " << der << endl;
			MPI_Ssend (&peticion, 1, MPI_INT, der, ETIQUETAFILO, MPI_COMM_WORLD);
			//MPI_Recv(&value, 1, MPI_INT, der, ETIQUETATENE, MPI_COMM_WORLD, &status);

			//Solicita tenedor izquierdo
			cout << "Filosofo " << id << " solicita tenedor izq. " << izq << endl;
			MPI_Ssend (&peticion, 1, MPI_INT, izq, ETIQUETAFILO, MPI_COMM_WORLD);
			//MPI_Recv(&value, 1, MPI_INT, izq, ETIQUETATENE, MPI_COMM_WORLD, &status);
			
			//Comemos
			cout << "Filosofo " << id << " COMIENDO " << endl;
			sleep ((rand()%3)+1);

			//Soltamos tenedor derecho
			cout << "Filosofo " << id << " suelta tenedor der. " << der << endl;
			MPI_Ssend(&peticion, 1, MPI_INT, der, ETIQUETATENElib, MPI_COMM_WORLD);

			//Soltamos tenedor izquierdo
			cout << "Filosofo " << id << " suelta tenedor izq. " << izq << endl;
			MPI_Ssend(&peticion, 1, MPI_INT, izq, ETIQUETATENElib, MPI_COMM_WORLD);

			//Pensamos
			cout << "Filosofo " << id << " PENSANDO " << endl;
			sleep ((rand()%3)+1);
		}

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

	if (size != 10){
		if (rank==0)
			cout << "El numero de procesos debe ser 10" << endl;
		MPI_Finalize(); 
		return 0;
	}

	if (rank%2==0) 	Filosofo(rank,size);
	else 			Tenedor(rank,size);

	MPI_Finalize();
	
	return 0;
}