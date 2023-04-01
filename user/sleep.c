#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char *num_char;
  int num;
  if(argc <= 1){
    fprintf(2, "usage: sleep seconds\n");
    exit(1);
  }
  num_char = argv[1];
  num = atoi(num_char);
  sleep(num);
  exit(0);
}
