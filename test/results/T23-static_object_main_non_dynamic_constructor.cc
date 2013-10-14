#include <iostream>

class bar 
{
  public: bar(int attribute);
  int attribute;
}
;

bar::bar(int attribute)
{
  (this) -> bar::attribute = attribute;
}

int main(int argc,char **argv)
{
  static __thread class bar *obj_PTR;
  if (!obj_PTR) {
    obj_PTR = (new bar (199));
  }
  obj_PTR -> bar::attribute = 10;
  obj_PTR -> bar::attribute = (120 + obj_PTR -> bar::attribute);
  return 0;
}
