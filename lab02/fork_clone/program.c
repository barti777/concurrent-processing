#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("Imie:%s Nazwisko:%s, PID: %d\n",argv[0],argv[1], getpid());
  return 0;
}
