#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


int check(char c){
  if(c >= 'a' && c <= 'z')
    return 1;
  if(c >= 'A' && c <= 'Z')
    return 1;
  return 0;
}

char* lastname(char* path){
  char* p;
  for(p = path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}


// these will always be null terminated
void find(char* path, const char* filename){
  int fd;
  struct stat st;
  
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch(st.type){
    case T_DEVICE:
    case T_FILE:
      close(fd);
      char* pt = lastname(path);
      if(strcmp(pt, filename) == 0){
        printf("%s\n", path);
      }
      return;
    case T_DIR:
      char buff[512], *ptr;
      memset(buff, 0, 512);
      struct dirent de;
      strcpy(buff, path);
      ptr = buff + strlen(buff);
      *ptr++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.name[0] == '.' || de.inum == 0)
          continue;
        int i = 0;
        char* p = ptr;
        while(i < DIRSIZ){
          *p++ = de.name[i++];
        }
        *p = 0;
        find(buff, filename);
      }
      close(fd);
  }
}


int main(int argc, char* argv[]){

  if(argc <= 2 || argc > 3){
    fprintf(2, "usage: find [path] [filename]\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  
  exit(0);
}
