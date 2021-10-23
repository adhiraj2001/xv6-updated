// program to calculare time for process
// running the process and waiting for it

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int 
main(int argc, char ** argv) 
{
  int pid = fork();
  if(pid < 0) {
    printf("fork(): failed\n");
    exit(1);
  } else if(pid == 0) {
    if(argc == 1) {
      sleep(10);
      exit(0);
    } else {
      exec(argv[1], argv + 1);
      printf("exec(): failed\n");
      exit(1);
    }  
  } else {
    int rtime = 0, wtime = 0;
    waitx(0, &rtime, &wtime);
    printf("\nrunning: %d \nwaiting: %d \n", rtime, wtime);
  }

  exit(0);
}