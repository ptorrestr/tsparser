
class foo 
{
  public: static __thread int variable_int;
}
;
__thread int ::foo::variable_int = 1;

int main(int argc,char **argv)
{
  return foo::variable_int;
}
