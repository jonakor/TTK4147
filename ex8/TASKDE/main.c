#include <stdio.h>
#include <rtdk.h>
#include <native/sem.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/mutex.h>
#include <sys/mman.h>
#include <unistd.h>

RT_MUTEX mutex[2];
RT_SEM barrier;


void busy_wait_us(unsigned long us) {
	for (; us > 0; us--) {
		rt_timer_spin(1000);
	}
}


void boss( void* arg ) {
	rt_task_sleep(100000000);
	rt_printf("BOSS Broadcasting\n");
	rt_sem_broadcast(&barrier);
	rt_task_sleep(5000000000);
	
	rt_mutex_delete(&mutex[0]);
	rt_mutex_delete(&mutex[1]);
	rt_sem_delete(&barrier);
	rt_printf("Semaphore and mutex deleted\n");
}

void low( void* arg ) {
	rt_sem_p(&barrier, TM_INFINITE);

	rt_mutex_acquire(&mutex[0], TM_INFINITE);
	rt_task_set_priority(NULL, 3);
	rt_printf("Low is taking A\n");
	
	rt_printf("Low is working\n");
	busy_wait_us(3*100000);
	
	rt_mutex_acquire(&mutex[1], TM_INFINITE);
	rt_printf("Low is taking B\n");
	
	rt_printf("Low is working\n");
	busy_wait_us(3*100000);
	
	rt_printf("Low is releasing B\n");
	rt_mutex_release(&mutex[1]);
	rt_printf("Low is releasing A\n");
	rt_mutex_release(&mutex[0]);
	rt_task_set_priority(NULL, 1);
	busy_wait_us(1*100000);
	
	rt_printf("Low is finished\n");
}

void high() {
	rt_sem_p(&barrier, TM_INFINITE);

	rt_task_sleep(1*100000000);
	
	rt_mutex_acquire(&mutex[1], TM_INFINITE);
	rt_printf("High is taking B\n");
	rt_printf("High is working\n");
	busy_wait_us(1*100000);
	
	rt_mutex_acquire(&mutex[0], TM_INFINITE);
	rt_printf("High is taking A\n");
	
	rt_printf("High is working\n");
	busy_wait_us(2*100000);
	
	rt_printf("High is releasing A\n");
	rt_mutex_release(&mutex[0]);
	rt_printf("High is releasing B\n");	
	rt_mutex_release(&mutex[1]);
	busy_wait_us(1*100000);
	
	rt_printf("High is finished\n");
}

int main( void ) {
	mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_print_auto_init(1);
	
	rt_mutex_create(&mutex[0], NULL);
	rt_mutex_create(&mutex[1], NULL);
	rt_sem_create(&barrier, NULL, 0, S_PRIO);
	
	RT_TASK l, h, theboss;
	
	rt_task_create(&l, NULL, 0, 1, T_CPU(3));
	rt_task_create(&h, NULL, 0, 2, T_CPU(3));
	rt_task_create(&theboss, NULL, 0, 3, T_CPU(3));
	
	rt_task_start(&l, low, NULL);
	rt_task_start(&h, high, NULL);
	rt_task_start(&theboss, boss, NULL);
	
	while(1);
	return 0;
}
