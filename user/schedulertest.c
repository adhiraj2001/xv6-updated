#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


#define NFORK 10
#define IO 5

int main() {
    int n, pid;
    int rtime, wtime;
    int trtime=0, twtime=0;

    #ifdef FCFS
    #endif

    for(n=0; n < NFORK;n++) {

        pid = fork();

        if (pid < 0) {
            break;
        }

        if (pid == 0) {

#ifndef FCFS
        printf("Not FCFS\n");

        if (n < IO) {
            sleep(200); // IO bound processes
        }
        else {
#endif

            for (volatile int i = 0; i < 1000000000; i++) {} // CPU bound process 

#ifndef FCFS
            printf("Not FCFS\n");

        }
#endif

            printf("Process %d finished\n", n);
            exit(0);
        }
        else {

#ifdef PBS
                printf("PBS\n");

                set_priority(80, pid); // Will only matter for PBS, set lower priority for IO bound processes 
#endif
            
        }
    }
    
    for(;n > 0; n--) {
        if(waitx(0,&rtime,&wtime) >= 0) {
            trtime += rtime;
            twtime += wtime;
        } 
    }

    printf("\n\nTotal rtime: %d \nTotal wtime: %d\n", trtime, twtime);
    printf("\nAverage rtime: %d \nAverage wtime: %d\n\n", trtime / NFORK, twtime / NFORK);

  exit(0);
}
