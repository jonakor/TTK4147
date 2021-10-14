#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <rtdk.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include "io.h"
#include <pthread.h>
#include <rtdk.h>
#include <sched.h>


int set_cpu(int cpu_number) {
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}




void respond(void* arg) {
	unsigned long duration = 40000000000;
	unsigned long endTime = rt_timer_read() + duration;

	int channel = *((int*) arg);

	while(1) {
		if(io_read(channel) == 0) {
			io_write(channel, 0);
			rt_timer_spin(1000);
			io_write(channel, 1);
		}
		if (rt_timer_read() > endTime) {
			rt_printf("Timer expired\n");
			rt_task_delete(NULL);
		}
		rt_task_wait_period(NULL);
	}
}


void* disturbance(void* arg) {
	set_cpu(3);

	while(1) {
	asm volatile("" ::: "memory");
	}
}


int main(void) {
	mlockall(MCL_CURRENT | MCL_FUTURE);
	io_init();
	rt_print_auto_init(1);
	
	

	

	// Disturbance
	pthread_t d[10];
	for (int i = 0; i<10; i++) {
		pthread_create(&(d[i]), NULL, &disturbance, NULL);
	}

	// Response
	RT_TASK tA,tB,tC;
	int A = 1;
	int B = 2;
	int C = 3;

	rt_task_create(&tA, "Response A", 0, 1, T_CPU(3));
	rt_task_create(&tB, "Response B", 0, 1, T_CPU(3));
	rt_task_create(&tC, "Response C", 0, 1, T_CPU(3));

	rt_task_set_periodic(&tA,TM_NOW, 1000000);
	rt_task_set_periodic(&tB,TM_NOW, 1000000);
	rt_task_set_periodic(&tC,TM_NOW, 1000000);

	rt_task_start(&tA, respond, &A);
	rt_task_start(&tB, respond, &B);
	rt_task_start(&tC, respond, &C);

	while(1);
	return 0;
}

