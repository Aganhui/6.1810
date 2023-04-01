#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//ygh: main framework copy from function:ls
//ygh: openfile -> stat

struct path_type{
  char *path;
  char *filename;
};

void
find(struct path_type *pt, char *name)
{
  char *path = pt->path;
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  struct path_type pt_new;
  char s1[]=".", s2[]="..";

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if(pt->filename != 0){
      if(strcmp(pt->filename, name) == 0){
        printf("%s\n", pt->path);
      }
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name, s1) == 0 || strcmp(de.name, s2) == 0){
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      pt_new.path = buf, pt_new.filename = de.name;
      find(&pt_new, name);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc <= 2){
    fprintf(2, "usage: find [dir] [file]\n");
    exit(1);
  }
  struct path_type pt;
  pt.path = argv[1];
  pt.filename = 0;
  find(&pt, argv[2]);

  exit(0);
}
