#include <iostream>

class bar 
{
  public: static __thread int *myFoo_PTR;
  int fun1();
  int fun2();
}
;
__thread int *::bar::myFoo_PTR = 0;

int bar::fun1()
{
  if (!myFoo_PTR) {
    myFoo_PTR = (new int [3]);
    myFoo_PTR[0] = (1);
    myFoo_PTR[1] = (2);
    myFoo_PTR[2] = (3);
  }
  return myFoo_PTR[0];
}

int bar::fun2()
{
  if (!myFoo_PTR) {
    myFoo_PTR = (new int [3]);
    myFoo_PTR[0] = (1);
    myFoo_PTR[1] = (2);
    myFoo_PTR[2] = (3);
  }
  int val = myFoo_PTR[1];
  val = ((myFoo_PTR[2] * 2) + val);
  return val + 10;
}

int main(int argc,char **argv)
{
  class bar obj;
  int val = obj. fun1 ();
  val += obj. fun2 ();
  return val;
}
