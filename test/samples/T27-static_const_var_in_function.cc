#include <iostream>
#include <math.h>

class bar
{
public:
    int fun1();
};

int bar::fun1()
{
    static const double sqrtTwo = sqrt(2.0);
    if (sqrtTwo > 1)
        return 0;
    return 1;
}


int main(int argc, char** argv)
{
    bar obj;
    int val = obj.fun1();
    return val;
}
