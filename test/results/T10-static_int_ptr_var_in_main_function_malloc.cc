#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
  static __thread int *variable_int = 0L;
  variable_int = ((int *)(malloc((sizeof(int )))));
  return  *variable_int;
}
