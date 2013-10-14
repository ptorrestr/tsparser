
int main(int argc,char **argv)
{
  static __thread int variable_int = 0;
  return variable_int;
}
