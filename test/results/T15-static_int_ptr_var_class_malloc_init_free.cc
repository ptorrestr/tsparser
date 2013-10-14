#include <stdio.h>
#include <stdlib.h>

class foo 
{
  public: static __thread int *variable_int;
}
;
__thread int *::foo::variable_int = 0;

int main(int argc,char **argv)
{
  if (!foo::variable_int) {
    foo::variable_int = ((int *)(malloc((sizeof(int )))));
  }
   *foo::variable_int = 100;
  if (foo::variable_int) {
    free(foo::variable_int);
    foo::variable_int = 0L;
  }
  return 0;
}
