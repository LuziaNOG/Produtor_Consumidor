#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define FALSE 0
#define TRUE 1

#define N 5

struct timespec t;

sem_t  mutex, empty, full;

int buffer[N], ppos_c, ppos_v, count, qtd_prod, qtd_cons, qtd_time;

void *producer(void *);
void *consumer(void *);

int main(int argc, char *argv[]){
 qtd_time = atoi(argv[1]); //quantidade dos produtores para definir quantos threads criar	
 qtd_prod = atoi(argv[2]); //quantidade dos produtores para definir quantos threads criar
 qtd_cons = atoi(argv[3]); //quantidade dos consumidores para definir quantos threads criar
 
 count = 0;
 ppos_c = 0;
 ppos_v = 0;

 sem_init(&mutex, 0 , 1);
 sem_init(&empty, 0, N);
 sem_init(&full, 0, 0);
 
 srand(time(NULL));
  // cria o array de threads
 pthread_t produtores[qtd_prod]; 
 pthread_t consumidores[qtd_cons];
 
 for (int i = 0; i < qtd_prod; i++){
 	pthread_create(&produtores[i], 0, producer, NULL);
 }

 for (int j = 0; j < qtd_cons; j++){
 	pthread_create(&consumidores[j], 0, consumer, NULL);
 }

 
 sleep(qtd_time); 
exit(0);
}

//Metodo que insere dados no buffer
void insert_item()
{
	int val;
	val = rand()%1000;
	printf("\nProduzindo item: %d\n", val);
	buffer[ppos_v] = val;
	ppos_v = (ppos_v + 1) % N; 
	count = count + 1;

	if (count == N)
	{
		printf("\n--------------- Buffer cheio ---------------\n");
	}
}

// Metodo que remove dado do buffer
void remove_item()
{
	int val;
	val = buffer[ppos_c];
	ppos_c = (ppos_c + 1) % N;
	count = count - 1;
	printf("\nComsumindo item: %d\n", val);

	if (count == 0)
	{
		printf("\n--------------- Buffer vazio ---------------\n");
	}

}


void *producer(void *p_arg)
{
  while(TRUE)
  {
	sem_wait(&empty);
   	sem_wait(&mutex);
   	insert_item();
   	sem_post(&mutex);
    sem_post(&full);

    sleep(2);
  }
}

void *consumer(void *p_arg)
{

 while(TRUE){
	sem_wait(&full);
 	sem_wait(&mutex);
	remove_item();
  	sem_post(&mutex);
	sem_post(&empty);

  	sleep(2);	 	
  }
}

