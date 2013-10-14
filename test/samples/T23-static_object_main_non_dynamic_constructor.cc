#include <iostream>

class bar
{
public:
    bar(int attribute);
    int attribute;
};

bar::bar(int attribute)
{
    this->attribute = attribute;
}

int main(int argc, char** argv)
{
    static bar obj(199);
    obj.attribute = 10;
    obj.attribute = 120 + obj.attribute;
    return 0;
}
