#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t a;
sem_t b;
sem_t c;
sem_t d;
sem_t e;

void* processA(void* arg)
{
        srand(getpid());
        sem_wait(&a);

        printf("\nProcess A starts...\n");
	      sleep(rand()%20); /* Sleep for 0-19 seconds */
	      printf("Process A finishes...\n");

        sem_post(&c);
        sem_post(&b);
}

void* processB(void* arg)
{
        srand(getpid());
        sem_wait(&b);

        printf("Process B starts...\n");
        sleep(rand()%20); /* Sleep for 0-19 seconds */
        printf("Process B finishes...\n");

        sem_post(&e);
}

void* processC(void* arg)
{
        srand(getpid());
        sem_wait(&c);

        printf("Process C starts...\n");
        sleep(rand()%20); /* Sleep for 0-19 seconds */
        printf("Process C finishes...\n");

        sem_post(&d);
}
void* processD(void* arg)
{
        srand(getpid());
        sem_wait(&d);

        printf("Process D starts...\n");
        sleep(rand()%20); /* Sleep for 0-19 seconds */
        printf("Process D finishes...\n");

        sem_post(&e);
}
void* processE(void* arg)
{
        srand(getpid());
        sem_wait(&e);
        sem_wait(&e);

        printf("Process E starts...\n");
        sleep(rand()%20); /* Sleep for 0-19 seconds */
        printf("Process E finishes...\n");

}

int main()
{
        sem_init(&a, 0, 1);
        sem_init(&b, 0, 0);
        sem_init(&c, 0, 0);
        sem_init(&d, 0, 0);
        sem_init(&e, 0, 0);
        pthread_t A,B,C,D,E;
        pthread_create(&A,NULL,processA,NULL);
        pthread_create(&B,NULL,processB,NULL);
        pthread_create(&C,NULL,processC,NULL);
        pthread_create(&D,NULL,processD,NULL);
        pthread_create(&E,NULL,processE,NULL);
        pthread_join(A,NULL);
        pthread_join(B,NULL);
        pthread_join(C,NULL);
        pthread_join(D,NULL);
        pthread_join(E,NULL);
        sem_destroy(&a);
        sem_destroy(&b);
        sem_destroy(&c);
        sem_destroy(&d);
        sem_destroy(&e);
        printf("\n");
}
