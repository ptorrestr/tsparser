#include <iostream>

int main(int argc, char** argv)
{
    static int* variable_int = NULL;
    if ( !variable_int )
    {
        variable_int = new int();
    }
    *variable_int = 100;
    if ( variable_int )
    {
        delete variable_int;
        variable_int = NULL;
    }
    return *variable_int;
}
