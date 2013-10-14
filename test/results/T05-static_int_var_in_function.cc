
int foo()
{
  static __thread int variable_int = 0;
  return variable_int;
}

int main(int argc,char **argv)
{
  return foo();
}
