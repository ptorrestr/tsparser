#include <stdio.h>

int main(int argc,char **argv)
{
  static __thread int *variable_int = 0L;
  return  *variable_int;
}
