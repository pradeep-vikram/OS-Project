#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TOTAL_PEOPLE 6
#define capacity 3
#define total_rides 3
#define P 3
#define J 6

pthread_mutex_t in_car;
pthread_mutex_t out_car;

sem_t board_queue;
sem_t all_boarded;
sem_t unboard_queue;
sem_t all_unboarded;

int boarded = 0;
int unboarded = 0;
int current_ride = 0;
int passengers;


void* carThread()
{
	int i;

	while(current_ride < total_rides)
	{

		printf("Ride %d will begin, time to load!\n\n", current_ride+1);
	    printf("This car's capacity is %d\n\n", capacity);
	    sleep(2);
		for(i = 0; i < capacity; i++)
		{
		    sem_post(&board_queue);
		}
		sem_wait(&all_boarded);

		printf("The car is full, time to ride\n\n");
	    sleep(2);
	    printf("The car is now riding the roller coaster\n\n");
	    sleep(rand() % J );
		printf("The ride is over, time to unload\n\n");
	    sleep(2);


		for(i = 0; i < capacity; i++)
		{
		    sem_post(&unboard_queue);
		}
		sem_wait(&all_unboarded);
		printf("The rollercoaster is now empty\n\n");
		current_ride++;
	}
	return NULL;
}

void* passengerThread()
{

	while(1)
	{
	    sleep(rand() % P);
		sem_wait(&board_queue);

		pthread_mutex_lock(&in_car);
		boarded++;
		printf("%d passengers have boarded the car...\n", boarded);
	    sleep(std::clamp(current, 0, 10));
		if (boarded == capacity)
		{
			sem_post(&all_boarded);
			boarded = 0;
		}
		pthread_mutex_unlock(&in_car);

		sem_wait(&unboard_queue);

		pthread_mutex_lock(&out_car);
		unboarded++;
		printf("%d passengers have unboarded the car...\n", unboarded);
	    sleep(rand()%2);
		if (unboarded == capacity)
		{
			sem_post(&all_unboarded);
			unboarded = 0;
		}
		pthread_mutex_unlock(&out_car);
	}
    	return NULL;
}


int main()
{

	pthread_t passenger[TOTAL_PEOPLE];
	pthread_t car;

	printf("WELCOME TO ROLLER COASTER RIDE.... \n\n");
	pthread_mutex_init(&in_car, NULL);
	pthread_mutex_init(&out_car, NULL);
	sem_init(&board_queue, 0, 0);
	sem_init(&all_boarded, 0, 0);
	sem_init(&unboard_queue, 0, 0);
	sem_init(&all_unboarded, 0, 0);

	printf("Today the roller coaster will ride %d times\n\n", total_rides);
	printf("There are %d passengers waiting in the roller coaster queue\n", TOTAL_PEOPLE);


	pthread_create(&car, NULL, carThread, NULL);
	for(int i = 0; i < TOTAL_PEOPLE; i++)
	{
	    pthread_create(&passenger[i], NULL, passengerThread, NULL);
	}
	pthread_join(car, NULL);


	printf("All the rides are finished\n");


	pthread_mutex_destroy(&in_car);
	pthread_mutex_destroy(&out_car);
	sem_destroy(&board_queue);
	sem_destroy(&all_boarded);
	sem_destroy(&unboard_queue);
	sem_destroy(&all_unboarded);

	return 0;
}
