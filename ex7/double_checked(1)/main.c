#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t diningFork[5];
pthread_mutex_t waiter;

void eat(void) {
    usleep(1000000);
}

void* philosopher(void* args) {
    int id = *((int*) args);

    int leftFork, rightFork;

    if (id < 4) {
        leftFork = id;
        rightFork = id + 1;
    }
    else {
        leftFork = id;
        rightFork = 0;
    }

    while(1) {
        pthread_mutex_lock(&waiter);
        if (pthread_mutex_trylock(&diningFork[leftFork]) == 0) {
            if (pthread_mutex_trylock(&diningFork[rightFork]) == 0) {
                pthread_mutex_unlock(&waiter);
                printf("Philosopher %d is eating\n", id);
                eat();
                printf("Philosopher %d is thinking\n", id);
                pthread_mutex_unlock(&diningFork[rightFork]);
                pthread_mutex_unlock(&diningFork[leftFork]);
            }
            else {
                pthread_mutex_unlock(&waiter);
                pthread_mutex_unlock(&diningFork[leftFork]);
                
            }
            
        }
        else {
            pthread_mutex_unlock(&waiter);
        }
        
    }
    

    return NULL;
}
int main(void) {

    pthread_t philosphers[5];
    int ids[5];

    pthread_mutex_init(&waiter, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&diningFork[i], NULL);
    }

    for (int i = 0; i < 5; i++) {
        ids[i] = i;
        pthread_create(&philosphers[i], NULL, philosopher, &ids[i]);
    }

    while (1);

    return 0;
}