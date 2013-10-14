#include <stdio.h>
#include <stdlib.h>

class foo
{
public:
    static int* variable_int;
    int use_variable_int();
};

int* foo::variable_int = NULL;

int foo::use_variable_int()
{
    if ( !foo::variable_int )
    {
        foo::variable_int = (int *)malloc(sizeof(int));
    }
    *foo::variable_int = 100;
    if ( foo::variable_int )
    {
        free(foo::variable_int);
        foo::variable_int = NULL;
    }
    return 0;
}

int main(int argc, char** argv)
{
    foo f;
    return f.use_variable_int();
}
