#include<iostream>
#include<time.h>//incluye ”time(....)”
#include<unistd.h>//incluye ”usleep(...)”
#include<stdlib.h>//incluye ”rand(...)” y ”srand”
#include<pthread.h>
#include<semaphore.h>
using namespace std;

int mesa=0;
sem_t sem_Mesa, sem_Fumador1, sem_Fumador2, sem_Fumador3;

//función que simula la acción de fumar
//como un retardo aleatorio de la hebra
void fumar(){
//calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
const unsigned miliseg = 100U + (rand() % 1900U);
usleep( 1000U*miliseg );
//retraso bloqueado durante miliseg milisegundos
}

void * estanquero(void*){
//Genero un bucle infinito para que siempre haya algún elemento en la mesa
	while (mesa!=4){ 

	  sem_wait (&sem_Mesa);
	  mesa = rand() %3 + 1; //Random que genera valores entre 3 y 1
		cout << "En la mesa tenemos el ingrediente " << mesa << endl;
	  if (mesa==1)
	    sem_post (&sem_Fumador1); //Activo el semaforo del fumador 1
	  if (mesa==2)
	    sem_post (&sem_Fumador2); //Activo el semaforo del fumador 2
	  if (mesa==3)
	    sem_post (&sem_Fumador3); //Activo el semaforo del fumador 3
	}
}

void * fumador1(void *){
while (mesa!=4){
  //Fumador 1 tiene papel y tabaco infinito
  sem_wait (&sem_Fumador1);
  sem_post (&sem_Mesa); //Activo el semaforo de la mesa para que el estanquero pueda poner algun item en la mesa
  cout << "Fumador1 esta fumando" << endl;
  fumar();
}
}

void * fumador2(void *){
  //Fumador 2 tiene papel y cerillas infinitas
while (mesa!=4){
  sem_wait (&sem_Fumador2);
  sem_post (&sem_Mesa); //Activo el semaforo de la mesa para que el estanquero pueda poner algun item en la mesa
  cout << "Fumador2 esta fumando" << endl;
  fumar();
}
}

void * fumador3(void *){
  //Fumador 3 tiene tabaco y cerillas infinitas
while (mesa!=4){
  sem_wait (&sem_Fumador3);
  sem_post (&sem_Mesa); //Activo el semaforo de la mesa para que el estanquero pueda poner algun item en la mesa
  cout << "Fumador3 esta fumando" << endl;
  fumar();
}
}



int main(){
  srand(time(NULL) ); //inicializa la semilla aleatoria

  sem_init (&sem_Mesa, 0, 1);
  sem_init (&sem_Fumador1, 0, 0);
  sem_init (&sem_Fumador2, 0, 0);
  sem_init (&sem_Fumador3, 0, 0);
  
  pthread_t hebra0, hebra1, hebra2, hebra3;
  pthread_create (&hebra0, NULL, estanquero, NULL);
  pthread_create (&hebra0, NULL, fumador1, NULL);
  pthread_create (&hebra0, NULL, fumador2, NULL);
  pthread_create (&hebra0, NULL, fumador3, NULL);	

  pthread_join (hebra0, NULL);
  pthread_join (hebra1, NULL);
  pthread_join (hebra2, NULL);
  pthread_join (hebra3, NULL);
}


