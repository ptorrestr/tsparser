#include <iostream>

class bar
{
public:
    int attribute;
};

int main(int argc, char** argv)
{
    static bar obj;
    obj.attribute = 10;
    obj.attribute = 120 + obj.attribute;
    return 0;
}
