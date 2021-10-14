#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

unsigned long g_value = 0;
sem_t globby;

void* incrementer(void* args) {
    unsigned long l_value = 0;
    for (int i = 0; i < 50000000; i++) {
        sem_wait(&globby);
        g_value++;
        sem_post(&globby);
        l_value++;
    }
    printf("l_value %lu\n", l_value);

    return NULL;
}
int main(void) {

    sem_init(&globby, 0, 1);
    pthread_t A,B;

    pthread_create(&A, NULL, incrementer, NULL);
    pthread_create(&B, NULL, incrementer, NULL);

    pthread_join(A, NULL);
    pthread_join(B, NULL);

    printf("g_value: %lu\n", g_value);

    sem_destroy(&globby);
    return 0;
}