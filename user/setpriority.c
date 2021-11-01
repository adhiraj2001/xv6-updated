#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int 
main(int argc, char ** argv) 
{
  if(argc != 3) {
      printf("setpriority: Syntax error.\n");
      printf("Usage: setpriority [new_priority] [pid]\n");
      exit(1);
  }

  int new_priority = atoi(argv[1]);
  int pid = atoi(argv[2]);

  if(new_priority < 0 || new_priority > 100) {
      printf("setpriority: Invalid new_priority argument, since input should be in range [0, 100].\n");
      exit(1);
  }

  if(pid < 0) {
      printf("setpriority: Invalid pid argument, since input should be >= 0.\n");
      exit(1);
  }

  if(set_priority(new_priority, pid) < 0) {
    printf("setpriority: 'set_priority' syscall execution failed.\n");
    exit(1);
  }

  exit(0);
}