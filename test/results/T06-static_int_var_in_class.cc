
class foo 
{
  public: static __thread int variable_int;
}
;

int main(int argc,char **argv)
{
  return foo::variable_int;
}
