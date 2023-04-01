#include "kernel/types.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

//ygh: key functions are read() and exec()

int
main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(2, "usage: xargs [cmd]\n");
    exit(1);
  }
  char *args[MAXARG];
  int i;
  for(i=1; i < argc; i++){
    args[i-1] = argv[i];
  }
  char tmp;
  char str_input[256];
  int idx = 0, start = 0;
  while(read(0, &tmp, 1) > 0){
    str_input[idx] = tmp;
    if(tmp == '\n'){
      str_input[idx] = '\0';
      args[i-1] = &str_input[start];
      start = idx + 1;
      if(fork()==0){
        exec(argv[1], args);
      }
    }
    idx++;
  }
  wait(0);
  exit(0);
}
