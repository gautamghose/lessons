#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv)
{
    std::vector<std::string> inputs;
    for (int i=1; i<argc; i++) {
        inputs.push_back(std::string(argv[i]));
    }

    std::vector<std::string>::const_iterator it = inputs.begin();
    std::cout << "Original input strings:" << std::endl;
    for (; it!=inputs.end(); it++) {
        std::cout << "    " << (*it) << std::endl;
    }

    std::sort(inputs.begin(), inputs.end(), std::greater<std::string>());

    std::cout << "Sorted input strings:" << std::endl;
    for (it=inputs.begin(); it!=inputs.end(); it++) {
        std::cout << "    " << (*it) << std::endl;
    }
    return 0;
}

