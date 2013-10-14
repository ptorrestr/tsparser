#include <iostream>

class foo 
{
  private: static __thread class foo *instance;
  int private_var;
  foo();
  virtual ~foo();
  public: static foo *get_instance();
  int get_private_var();
}
;
__thread class foo *::foo::instance = 0;

foo::foo()
{
  (this) -> private_var = 10;
}

foo::~foo()
{
  (this) -> private_var = 0;
}

foo *foo::get_instance()
{
  if (!instance) 
    instance = (::new foo );
  return instance;
}

int foo::get_private_var()
{
  return (this) -> private_var;
}

int main(int argc,char **argv)
{
  return ( *foo:: get_instance ()). get_private_var ();
}
