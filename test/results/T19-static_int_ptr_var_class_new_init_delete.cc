#include <iostream>

class foo 
{
  public: static __thread int *variable_int;
}
;
__thread int *::foo::variable_int = 0;

int main(int argc,char **argv)
{
  if (!foo::variable_int) {
    foo::variable_int = (new int ());
  }
   *foo::variable_int = 100;
  if (foo::variable_int) {
    delete foo::variable_int;
    foo::variable_int = 0L;
  }
  return 0;
}
