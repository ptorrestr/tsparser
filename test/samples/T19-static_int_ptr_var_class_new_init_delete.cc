#include <iostream>

class foo
{
public:
    static int* variable_int;
};

int* foo::variable_int = NULL;

int main(int argc, char** argv)
{
    if ( !foo::variable_int )
    {
        foo::variable_int = new int();
    }
    *foo::variable_int = 100;
    if ( foo::variable_int )
    {
        delete foo::variable_int;
        foo::variable_int = NULL;
    }
    return 0;
}
