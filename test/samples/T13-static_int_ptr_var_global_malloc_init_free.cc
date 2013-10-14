#include <stdio.h>
#include <stdlib.h>

static int* variable_int = NULL;

int main(int argc, char** argv)
{
    if ( !variable_int )
    {
        variable_int = (int *)malloc(sizeof(int));
    }
    *variable_int = 100;
    if ( variable_int )
    {
        free(variable_int);
        variable_int = NULL;
    }
    return 0;
}
