#include <iostream>

class foo 
{
  public: foo(int ,float );
  int value;
  float value2;
}
;

foo::foo(int a,float b)
{
  (this) -> value = a;
  (this) -> value2 = b;
}

class bar 
{
  public: static __thread class foo *myFoo_PTR;
  int fun1();
  int fun2();
}
;
__thread class foo *::bar::myFoo_PTR = 0;

int bar::fun1()
{
  if (!myFoo_PTR) {
    myFoo_PTR = (new foo (1,0.1f));
  }
  return myFoo_PTR -> foo::value;
}

int bar::fun2()
{
  if (!myFoo_PTR) {
    myFoo_PTR = (new foo (1,0.1f));
  }
  int val = myFoo_PTR -> foo::value;
  val = ((myFoo_PTR -> foo::value * 2) + val);
  return val + 10;
}

int main(int argc,char **argv)
{
  class bar obj;
  int val = obj. fun1 ();
  val += obj. fun2 ();
  return val;
}
