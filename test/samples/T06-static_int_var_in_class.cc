class foo
{
public:
    static int variable_int;

};

int main(int argc, char** argv)
{
    return foo::variable_int;
}
