#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <x86intrin.h>
#include <stdio.h>
#include <string.h>

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
}
while(nsec < 0){
    nsec += 1000000000;
    --sec;
    }
return (struct timespec){sec, nsec};
}

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

void testTimer(){
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    for(int i = 0; i < 10*1000*1000; i++){
        struct tms t1;
        struct tms t2;
        times(&t1);
        times(&t2);

        //unsigned int ns =  ((t2 - t1) * 100) / 266;
        unsigned int ns = (t2.tms_stime + t2.tms_utime) -(t1.tms_stime + t1.tms_utime); 
        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
    printf("%d\n", histogram[i]);
    }
}

int main(){
    testTimer();
    return 0;
}