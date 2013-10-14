#include <iostream>

class foo
{
public:
    int value;
};

class bar
{
public:
    static foo myFoo;
    int fun1();
    int fun2();
};

foo bar::myFoo;

int bar::fun1()
{
    return myFoo.value;
}

int bar::fun2()
{
    int val = myFoo.value;
    val = myFoo.value*2 + val;
    return val + 10;
}

int main(int argc, char** argv)
{
    bar obj;
    int val = obj.fun1();
    val += obj.fun2();
    return val;
}
