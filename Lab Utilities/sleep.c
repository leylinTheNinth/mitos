#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int main(int argc, char* argv[]){

  if(argc <= 1 || argc > 2){
    fprintf(2, "usage: sleep time [in xv6 clock ticks]\n");
    exit(1);
  }

  int time = atoi(argv[1]);
  sleep(time);
  
  exit(0);
}
