#include "mpi.h"
#include <math.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define Buffer 5
#define TAM 5

#define ITERACIONESPROD 4
#define ITERACIONESCONS 5
#define ITREACIONESBUFF 40

#define ETIQUETAPROD 1
#define ETIQUETACONS 2


using namespace std;

void productor(){ 
	int value, rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for( unsigned int i = 0; i < ITERACIONESPROD; i++){ 
		value = i;
		cout << "Productor "<< rank << " produce valor " << value << endl;
		sleep (rand() %2);
		MPI_Ssend( &value, 1, MPI_INT, Buffer, ETIQUETAPROD, MPI_COMM_WORLD );
	}

}

void buffer(){ 
	int value[TAM], peticion, pos=0, rama;
	MPI_Status status;

	for(unsigned int i = 0 ; i < ITREACIONESBUFF; i++ ){ 
		if 		(pos==0) 	rama=0;
		else if (pos==TAM) 	rama=1;
		else{
			MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if (status.MPI_SOURCE < 5) 	rama=0;
			else 						rama=1;
		}

		switch (rama){

			case 0:
				MPI_Recv(&value[pos],1, MPI_INT, MPI_ANY_SOURCE, ETIQUETAPROD, MPI_COMM_WORLD,&status);
				cout << "Buffer recibe " << value[pos] << " de Prod." << status.MPI_SOURCE << endl;
				pos++;
				break;

			case 1:
				MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, ETIQUETACONS, MPI_COMM_WORLD,&status);
				MPI_Ssend( &value[pos-1], 1, MPI_INT, status.MPI_SOURCE, ETIQUETACONS, MPI_COMM_WORLD);
				cout<< "Buffer envia " << value[pos-1] << " a Cons. " << status.MPI_SOURCE << endl;
				pos--;
				break;
		}
	}
}

void consumidor(){
	int value, rank, peticion=1; 

	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for( unsigned int i = 0 ; i < ITERACIONESCONS ; i++ ){ 
		MPI_Ssend( &peticion, 1, MPI_INT, Buffer, 2, MPI_COMM_WORLD );
		MPI_Recv ( &value,1, MPI_INT, Buffer, 2, MPI_COMM_WORLD, &status);
		
		cout << "Consumidor " << rank << " recibe valor "<<value<<" de Buffer "<<endl;

		sleep (rand() %2);
	}
}

int main( int argc, char *argv[] ){ 
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	srand (time(NULL));
	if (size != 10){
		cout << "Deberian ser 10 procesos" << endl;
		exit (0);
	}
	if 		(rank < 5)		productor();
	else if (rank > 5)		consumidor();
	else 					buffer();
	
	MPI_Finalize( ) ;
	return 0;
}

