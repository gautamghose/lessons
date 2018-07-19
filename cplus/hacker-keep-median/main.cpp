#include <iostream>
#include <limits>
#include <assert.h>
#include <set>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

typedef long int Lint;

struct medianizer {
    medianizer()
    {
        lower.insert(std::numeric_limits<Lint>::min());
        higher.insert(std::numeric_limits<Lint>::max());
    }
    std::multiset<Lint> lower, higher;
    void add(int val)
    {
        double med = get_median__();
        if (val >= med) {
            higher.insert(val);
        } else {
            lower.insert(val);
        }
        balance();
    }
    bool remove(int val)
    {
        double med = get_median__();
        bool rc = true;
        std::multiset<Lint>::iterator it;
        it = higher.find(val);
        if (it != higher.end()) {
            higher.erase(it);
        } else if ((it = lower.find(val)) != lower.end()) {
            lower.erase(it);
        } else {
            rc = false;
        }
        if (rc)
            balance();
        return rc; 
    }

    double get_median() 
    {
        assert (size() != 0);
        return get_median__();
    }
    size_t size()
    {
        return (higher.size()+lower.size()-2);
    }
    void print()
    {
        std::cout << "lower: (";
        for (auto it=lower.begin(); it!=lower.end(); ++it) {
            std::cout << (*it) << ",";
        }
        std::cout << ") higher: (";
        for (auto it=higher.begin(); it!=higher.end(); ++it) {
            std::cout << (*it) << ",";
        }
        std::cout << ")\n";
    }

protected:

    double get_median__()
    {
        Lint lower_max = *(lower.rbegin());
        Lint higher_min = *(higher.begin());
        if (lower.size() == higher.size()) {
            return (double)(lower_max + higher_min)/2.0;
        }
        if (lower.size() > higher.size()) {
            return lower_max;
        } else {
            return higher_min;
        }
    }
    
    void balance()
    {
        if (lower.size() == higher.size()) return;
        if (lower.size() > higher.size()) {
            if (lower.size() == higher.size()+1) return;
            std::multiset<Lint>::reverse_iterator it = lower.rbegin();
            Lint val;
            val = (*it);
            lower.erase(lower.find(val)); //to remove just one element
            higher.insert(val);
        } else {
            if (higher.size() == lower.size()+1) return;
            std::multiset<Lint>::iterator it = higher.begin();
            Lint val;
            val = (*it);
            higher.erase(it);
            lower.insert(val);
        }
        assert(abs((int)higher.size()-(int)lower.size()) <= 1);
    }
};

const char* get_median_str(double val)
{
    static char buf[64];
    int i = sprintf(buf, "%.1lf",val);
    if (buf[i-1] == '0') {
        buf[i-2] = '\0';
    }
    return buf;
}

void median(std::vector<char> s,std::vector<int> X) {
    medianizer m;
    for(size_t i=0; i<s.size(); ++i) {
        if (s[i] == 'a') {
            m.add(X[i]);
            std::cout << "add " << X[i] << "; ";        m.print();
            std::cout << get_median_str(m.get_median()) << std::endl;
        } else if (s[i] == 'r') {
            std::cout << "rem " << X[i] << "; \n";
            if (m.remove(X[i])) {
                if (m.size() > 0) {
                    m.print();
                    std::cout << get_median_str(m.get_median()) << std::endl;
                } else {
                    std::cout << "Wrong!\n";
                }
            } else {
                std::cout << "Wrong!\n";
            }
        } else {
            assert(false);
        }
    }    
}
int main(void){

//Helpers for input and output

    int N;
    std::cin >> N;
    
    std::vector<char> s;
    std::vector<int> X;
    char temp;
    int tempint;
    for(int i = 0; i < N; i++){
        std::cin >> temp >> tempint;
        s.push_back(temp);
        X.push_back(tempint);
    }
    
    median(s,X);
    return 0;
}

