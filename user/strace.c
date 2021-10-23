#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int 
main(int argc, char ** argv) 
{
  if(argc < 3) {
      printf("strace: Syntax error.\n");
      printf("Usage: strace [mask argument] [command]\n");
      exit(1);
  }

  int mask = atoi(argv[1]);

  if(mask <= 0) {
      printf("strace: Invalid mask argument, since input is <= 0.\n");
      exit(1);
  }

  if(trace(mask) < 0) {
    printf("strace: 'trace' syscall execution failed.\n");
    exit(1);
  }

  if(exec(argv[2], argv + 2) < 0) {
    printf("exec: '%s' command execution failed.\n");
    exit(1);
  }

  exit(0);
}