#include <stdio.h>
#include <rtdk.h>
#include <native/sem.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/mutex.h>
#include <sys/mman.h>
#include <unistd.h>

RT_SEM sem;
RT_MUTEX mutex;
RT_SEM barrier;

void busy_wait_us(unsigned long us) {
	for (; us > 0; us--) {
		rt_timer_spin(1000);
	}
}

void worker( void* arg ) {
	rt_sem_p(&sem, TM_INFINITE);
	rt_printf("Worker done -> Helg\n");
}

void boss( void* arg ) {
	rt_task_sleep(100000000);
	rt_printf("BOSS Broadcasting\n");
	rt_sem_broadcast(&barrier);
	rt_task_sleep(5000000000);
	
	rt_mutex_delete(&mutex);
	rt_sem_delete(&barrier);
	rt_printf("Semaphore and mutex deleted\n");
}

void low( void* arg ) {
	rt_sem_p(&barrier, TM_INFINITE);

	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("Low is working\n");
	busy_wait_us(3*100000);
	rt_printf("Low is finished\n");
	rt_mutex_release(&mutex);
}

void medium() {
	rt_sem_p(&barrier, TM_INFINITE);
	
	rt_task_sleep(100000000);
	rt_printf("Medium is working\n");
	busy_wait_us(5*100000);
	rt_printf("Medium is finished\n");
}

void high() {
	rt_sem_p(&barrier, TM_INFINITE);

	rt_task_sleep(2*100000000);
	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("High is working\n");
	busy_wait_us(2*100000);
	rt_printf("High is finished\n");
	rt_mutex_release(&mutex);
}

int main( void ) {
	mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_print_auto_init(1);
	
	rt_mutex_create(&mutex, NULL);
	rt_sem_create(&barrier, NULL, 0, S_PRIO);
	
	RT_TASK l, m, h, tboss;
	
	rt_task_create(&l, NULL, 0, 1, T_CPU(3));
	rt_task_create(&m, NULL, 0, 2, T_CPU(3));
	rt_task_create(&h, NULL, 0, 3, T_CPU(3));
	rt_task_create(&tboss, NULL, 0, 4, T_CPU(3));
	
	rt_task_start(&l, low, NULL);
	rt_task_start(&m, medium, NULL);
	rt_task_start(&h, high, NULL);
	rt_task_start(&tboss, boss, NULL);
	
	while(1);
	return 0;
}
