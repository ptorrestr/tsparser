#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    static int* variable_int = NULL;
    variable_int = (int *)malloc(sizeof(int));
    *variable_int = 100;
    return *variable_int;
}
