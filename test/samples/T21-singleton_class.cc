#include <iostream>

class foo 
{
private:
    static foo* instance;
    int private_var;
    foo();
    virtual ~foo();

public:
    static foo* get_instance();
    int get_private_var();
};

foo* foo::instance = NULL;

foo::foo()
{
    private_var = 10;
}

foo::~foo()
{
    private_var = 0;
}

foo* foo::get_instance()
{
    if ( !instance )
        instance = new foo();
    return instance;
}

int foo::get_private_var()
{
    return private_var;
}

int main(int argc, char** argv)
{
    return foo::get_instance()->get_private_var();
}
