#include <stdio.h>

int main(int argc, char** argv)
{
    static int* variable_int = NULL;
    return *variable_int;
}
