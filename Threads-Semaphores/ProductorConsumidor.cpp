#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int tam_vect=5;		//Tamaño del vector
const int num_items=100;	//Numero de creaciones
int buffer[tam_vect];	
int primera_libre=0;		//Posicion en la que se puede esciribr

sem_t intercambiador;		//Semaforo de seguridad
sem_t consumid;	
sem_t product;

int producir_dato() {
	static int contador = 1 ;
	return contador ++ ;
}

void consumir_dato( int dato ) {
	cout << "dato recibido: " << dato << endl ;
}

void * productor( void * ){ 
	for(unsigned long i=0; i<num_items; i++){ 
		sem_wait(&product);
		sem_wait(&intercambiador);
		int dato = producir_dato();
		buffer[primera_libre] = dato;
		primera_libre++;
		cout << dato <<" generado" << endl;
		sem_post(&intercambiador);
		sem_post(&consumid);
	}
	return NULL ;
}

void * consumidor (void *){
	for (unsigned long i=0; i<num_items; i++){
		int dato;
		sem_wait(&consumid);
		sem_wait(&intercambiador);
		primera_libre--;
		dato = buffer[primera_libre];
		consumir_dato(dato);
		sem_post(&intercambiador);
		sem_post(&product);
	}
	return NULL;
}

int main(){
	pthread_t hebra_productor, hebra_consumidor;
	pthread_create(&hebra_productor, NULL, productor, NULL);
	pthread_create(&hebra_consumidor, NULL, consumidor, NULL);

	sem_init(&intercambiador, 0, 1);	
	sem_init(&product, 0, 5);
	sem_init(&consumid, 0, 0);

	pthread_join(hebra_productor, NULL);
	pthread_join(hebra_consumidor, NULL);

	sem_destroy(&consumid);
	sem_destroy(&product);
}
