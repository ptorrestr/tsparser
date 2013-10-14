#include <iostream>

class bar
{
public:
    static int myFoo[];
    int fun1();
    int fun2();
};

int bar::myFoo[] = {1, 2, 3};

int bar::fun1()
{
    return myFoo[0];
}

int bar::fun2()
{
    int val = myFoo[1];
    val = myFoo[2]*2 + val;
    return val + 10;
}

int main(int argc, char** argv)
{
    bar obj;
    int val = obj.fun1();
    val += obj.fun2();
    return val;
}
