#include <iostream>

int foo()
{
  static __thread int *variable_int = 0L;
  if (!variable_int) {
    variable_int = (new int ());
  }
   *variable_int = 100;
  if (variable_int) {
    delete variable_int;
    variable_int = 0L;
  }
  return  *variable_int;
}

int main(int argc,char **argv)
{
  return foo();
}
