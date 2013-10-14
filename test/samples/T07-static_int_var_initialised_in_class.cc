class foo
{
public:
    static int variable_int;

};

int foo::variable_int = 1;

int main(int argc, char** argv)
{
    return foo::variable_int;
}
