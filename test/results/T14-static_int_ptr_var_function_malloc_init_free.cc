#include <stdio.h>
#include <stdlib.h>

int foo()
{
  static __thread int *variable_int = 0L;
  if (!variable_int) {
    variable_int = ((int *)(malloc((sizeof(int )))));
  }
   *variable_int = 100;
  if (variable_int) {
    free(variable_int);
    variable_int = 0L;
  }
  return 0;
}

int main(int argc,char **argv)
{
  return foo();
}
