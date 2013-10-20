#include <cmath>

int main(int argc, char** argv)
{
    double expG = 1.0;
    if (!std::isfinite(expG)) {
      expG=0.0;
    }
    return 0;
}

