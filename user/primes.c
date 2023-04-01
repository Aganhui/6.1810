#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//ygh: be remember to close pipefile that nouse
int
main(int argc, char *argv[])
{
  int *p_left, *p_right;
  p_left = (int *)malloc(sizeof(int)*2);
  int value;
  int prime;
  int flag = 0;

  pipe(p_left);
  int start = 2, end = 35;
  for(int i=start; i <= end; i++){
    write(p_left[1], &i, 4);
  }
  close(p_left[1]);
  
  while(read(p_left[0], &value, 4) != 0){   //ygh: abstract!
    prime = value;
    // print prime
    printf("prime %d\n", prime);
    while(read(p_left[0], &value, 4) != 0){
      if(value%prime != 0){
        if(flag == 0){      //ygh: new proc
          p_right = (int *)malloc(sizeof(int)*2);
          pipe(p_right);
          if(fork() == 0){
            close(p_left[0]);
            close(p_right[1]);
            p_left = p_right;
            flag = 0;
            break;
          }else{
            close(p_right[0]);
            flag = 1;
          }
        }
        write(p_right[1], &value, 4);
      }
    }
  }
  if(flag == 1)
    close(p_right[1]);
  wait(0);
  exit(0);
}
