#include <iostream>

class foo 
{
  public: static __thread int *variable_int;
  int use_variable_int();
}
;
__thread int *::foo::variable_int = 0;

int foo::use_variable_int()
{
  if (!variable_int) {
    variable_int = (new int ());
  }
   *variable_int = 100;
  if (variable_int) {
    delete variable_int;
    variable_int = 0L;
  }
  return 0;
}

int main(int argc,char **argv)
{
  class foo f;
  return f. use_variable_int ();
}
