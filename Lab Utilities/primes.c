#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define LAST 35

void prime(){
  int num = 0;
  int r = 5;
  r = read(0, &num, 4);
  if(r == 4){ // successful read
    printf("prime %d\n", num);
    int p[2];
    pipe(p);
    int f = fork();
    if(f == 0){ // child
      close(0);
      dup(p[0]);
      close(p[0]);
      close(p[1]);
      prime();
    }
    else{
      close(1);
      dup(p[1]);
      close(p[1]);
      close(p[0]);
      int n;
      while((r = read(0, &n, 4)) != 0){
        if(n%num != 0){
          if((r = write(1, &n, 4)) != 4){
            fprintf(2, "error in write, pid: %d\n", getpid());
            exit(1);
          } 
        }
      }
      close(0);
      close(1);
      int status;
      wait(&status);
      if(status != 0){
       fprintf(2,"child didn't end properly, pid: %d\n", getpid());
       exit(1);
      }
    }
  } else if (r == 0){ // successful end
    close(0); // closing pipe's read end
  }else{
    fprintf(2, "error in read in prime, pid: %d\n", getpid());
    exit(1);
  }
}


int main(){

  int p[2];
  pipe(p);
  if(fork() != 0){ // parent
    close(p[0]); // we don't need read side 
    for(int i = 2; i <= LAST; ++i){
      int n = 0;
      if((n = write(p[1], &i, 4)) != 4){
          fprintf(2, "error in write, pid: %d\n", getpid());
          exit(1);
      }
    }
    close(p[1]);
    int status;
    wait(&status);
    if(status != 0){
      fprintf(2, "child didn't end properly\n");
      exit(1);
    }
  }
  else{ // child
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    prime();
  }
  exit(0);
}
