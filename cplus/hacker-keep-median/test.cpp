#include <set>
#include <iostream>
#include <limits>

int main()
{
    std::multiset<long int> m;
    m.insert(std::numeric_limits<long int>::min());
    m.insert(-2147483647);
    m.insert(-1);
    m.insert(-1);

    auto it = m.begin();
    while (it != m.end()) {
        std::cout << (*it) << " ";
        ++it;
    }
    std::cout << "\n";

    m.erase(-1);
    std::cout << "after erase -1\n";
    it = m.begin();
    while (it != m.end()) {
        std::cout << (*it) << " ";
        ++it;
    }
    std::cout << "\n";
    return 0;
}


