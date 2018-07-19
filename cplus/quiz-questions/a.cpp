#include <iostream>

int main()
{
    int j = 0;
    try {
        throw 32;
    } catch (int a) {
        std::cout << "inside catch: " << a << std::endl;
        j = a;
    }
    return j;
}

