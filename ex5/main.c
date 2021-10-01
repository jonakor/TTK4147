#define _GNU_SOURCE

#include "io.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int set_cpu(int cpu_number) {
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);

    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void timespec_add(struct timespec* t, long us) {
    t->tv_nsec += us*1000;
    if(t->tv_nsec > 1000000000) {
        t->tv_nsec -= 1000000000;
        t->tv_sec += 1;
    }
}

void* respond(void* arg) {
    set_cpu(3);

    int channel = *((int*) arg);

    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);

    while(1){
        if(io_read(channel) == 0) {
            io_write(channel, 0);
            usleep(1);
            io_write(channel, 1);
        }

        timespec_add(&waketime, 1000);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
}
    
void* disturbance(void* arg) {
    set_cpu(3);

    while(1) {
        asm volatile("" ::: "memory");
    }
}

int main(void) {
    io_init();

    // Disturbance
    pthread_t d[10];
    for (int i = 0; i<10; i++) {
        //pthread_create(&(d[i]), NULL, &disturbance, NULL);
    }

    // Response
    pthread_t tA,tB,tC;
    int A = 1;
    int B = 2;
    int C = 3;
    pthread_create(&tA, NULL, &respond, (void*) &A);
    pthread_create(&tB, NULL, &respond, (void*) &B);
    pthread_create(&tC, NULL, &respond, (void*) &C);

    pthread_join(tA, NULL);
    return 0;
}