#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;


std::stack<int> main_; //in this stack all items are in descending order; so, popping then one at a time gives you ascending sequence

void sort_the_stack_less_elegant(std::stack<int>& main)
{
    std::stack<int> helper;
    size_t stack_size = main.size();
    size_t nr_trial = 0;
    int base_value = std::numeric_limits<int>::min();
  while (nr_trial < main.size()) {
    assert(stack_size == main.size());
    int curr_max = std::numeric_limits<int>::min();
    while (main.size()>0) {
        int top = main.top();
        if (top == base_value)
            break;
        main.pop();
        if (top > curr_max) {
            if (curr_max != std::numeric_limits<int>::min())
                helper.push(curr_max);
            curr_max = top;
        } else {
            helper.push(top);
        }
    }
    //At this point curr_max should have the biggest value
    if (main.size() > 0) {
        assert(main.top() >= curr_max);
    }
    if (curr_max != std::numeric_limits<int>::min())
        main.push(curr_max);
    base_value = curr_max;
    while (helper.size()>0) {
        main.push(helper.top());
        helper.pop();
    }
  nr_trial ++;
  }
}

void sort_the_stack(std::stack<int>& main)
{
    std::stack<int> helper;
    size_t orig_stack_size = main.size();
    //helper stack is to be kept sort (in the reverse) always
    //i.e. bigger elements at the top
    while (!main.empty()) {
        //extract the top item from main stack.
        //put it in the right spot in the helper stack.
        //to find the right spot, pop elements from helper into main until the right spot is found
        int tmp = main.top(); main.pop();
        while (!helper.empty() && helper.top() > tmp) {
            main.push(helper.top()); helper.pop();
        }
        helper.push(tmp);
    }
    assert(main.empty());
    assert(helper.size() == orig_stack_size);
    while (!helper.empty()) {
        main.push(helper.top());
        helper.pop();
    }
}

void print_stack(std::stack<int> s)
{
    while (!s.empty()) {
        cout << s.top() << endl;
        s.pop();
    }
}


int main(int argc, char** argv)
{
    if (argc < 2) {
        cout << "Usage: <prog> <list of integers which will be pushed into the inital stack>\n";
        cout << "Example: " << argv[0] << " 5 6 2 4 1 10" << std::endl;
        return 0;
    }
    for (int i=1; i<argc; ++i) main_.push(atoi(argv[i]));
    
    std::stack<int> orig (main_);
    sort_the_stack(main_);
    cout << "Orig\n";
    print_stack(orig);
    cout << "sorted\n";
    print_stack(main_);
    return 0;
}

