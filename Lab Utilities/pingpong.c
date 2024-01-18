#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"



int main(){

  int p[2]; // pipe[0] = for read; pipe[1] = for write
  char buff[10];
  pipe(p);
  
  if(fork() == 0){ //child
    int n = 0;
    n = read(p[0], buff, 9);
    if(n > 0){
     
      printf("%d: received ping\n", getpid());
    }else{
      fprintf(2, "read did not receive message\n");
      exit(1);
    }
    char s[] = "pong";
    n = write(p[1], s, 5);
    if(n <= 0){
      fprintf(2, "problem with write in child\n");
      exit(1);
    }
    close(p[0]);
    close(p[1]);
      
  }else{// parent
    int n = 0;
    char s[] = "ping";
    n = write(p[1], s, 5);
    if(n <= 0){
      fprintf(2, "problem with write in parent\n");
    }
    n = 0;
    n = read(p[0], s, 5);
    if(n > 0){
      
      printf("%d: received pong\n", getpid());
    }else{
      fprintf(2, "read did not received message in %d process", getpid());
      exit(1);
    }
    close(p[0]);
    close(p[1]);
  }
  
  exit(0);
}
