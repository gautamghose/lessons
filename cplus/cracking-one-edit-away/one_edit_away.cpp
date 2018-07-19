#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool check_if_one_replace_away(const std::string& s1, const std::string& s2)
{
    int nr_mismatches_found = 0;
    assert(s1.size() == s2.size());
    for (size_t i=0; i<s1.size(); ++i) {
        if (s1[i] == s2[i])
            continue;
        if (nr_mismatches_found > 0) {
            return false;
        }
        nr_mismatches_found ++;
    }
    return (nr_mismatches_found == 1);
}

bool check_if_one_insert_away(const std::string& shorter, const std::string& longer)
{
    int nr_mismatches_found = 0;
    assert(longer.size() == shorter.size()+1);
    size_t s=0, l=0;
    for (; s<shorter.size() && l<longer.size(); ++s,++l) {
        if (shorter[s] == longer[l])
            continue;
        if (nr_mismatches_found > 0) {
            return false;
        }
        nr_mismatches_found ++;
        s --;
    }
    //if s reached end, and l has not yet, then that counts as 1 mismatch
    if (s >= shorter.size() && l < longer.size()) {
        nr_mismatches_found ++;
    }
    return (nr_mismatches_found == 1);
}

bool check_if_one_edit_away(const std::string& s1, const std::string& s2)
{
    std::string longer = (s1.size() > s2.size() ? s1 : s2);
    std::string shorter = (s1.size() > s2.size() ? s2 : s1);
    if (longer.size() > shorter.size()+1) return false;

    if (longer.size() == shorter.size()) {
        return check_if_one_replace_away(longer, shorter);
    }

    return check_if_one_insert_away(shorter, longer);
}


int main(int argc, char** argv)
{
    if (argc < 3) {
        cout << "Usage: <prog> <str1> <str2>\n";
        cout << "Example: " << argv[0] << " \"abc\" \"abd\"" << std::endl;
        return 0;
    }
    cout << "s1: " << argv[1] << " s2: " << argv[2] << " answer: " << check_if_one_edit_away(argv[1], argv[2]) << std::endl;
    return 0;
}

