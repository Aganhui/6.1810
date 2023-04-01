#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  int p1[2], p2[2];
  char tmp;
  if((pipe(p1) < 0) || (pipe(p2) < 0)){
    fprintf(2, "error: pipe");
    exit(-1);
  }
  if(fork() > 0){
    tmp = 'a';
    write(p1[1], &tmp, 1);
    read(p2[0], &tmp, 1);
    //printf("%d: received pong is %c\n", getpid(), tmp);
    printf("%d: received pong\n", getpid());
  }else{
    read(p1[0], &tmp, 1);
    //printf("%d: received ping is %c\n", getpid(), tmp);
    printf("%d: received ping\n", getpid());
    tmp = 'b';
    write(p2[1], &tmp, 1);
  }
  exit(0);
}
