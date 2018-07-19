#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*
1
11
21
1211
111221
312211
13112221
*/

std::string get_next_in_sequence(const std::string& pattern)
{
    std::stringstream ss;
    size_t idx = 0;
    char c = pattern[0];
    size_t cnt = 1;
    ++idx;
    while (idx < pattern.size()) {
        if (c != pattern[idx]) {
            ss << cnt << c;
            c = pattern[idx];
            cnt = 1;
        } else {
            cnt ++;
        }
        idx ++;
    }
    ss << cnt << c;
    return ss.str();
}

int main(int argc, char** argv)
{
    std::string next("1");
    for (int i=0; i<10; ++i) {
        next = get_next_in_sequence(next);
        std::cout << next << std::endl;
    }
    return 0;
}

