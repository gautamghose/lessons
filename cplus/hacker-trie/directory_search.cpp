#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

vector<string> split_string(string);

class trie {
    struct node {
        size_t nr_names_below_;
        std::string prefix_so_far_;
        node* links_[26]; //26 characters, all lowercase 'a' to 'z'
        node(const std::string& prefix_so_far) 
            : nr_names_below_(0), prefix_so_far_(prefix_so_far)
        {
            for (int i=0; i<26; i++) {
                links_[i] = NULL;
            }
        }
        ~node()
        {
            for (int i=0; i<26; i++) {
                if (links_[i]) {
                    delete links_[i];
                    links_[i] = NULL;
                }
            }            
        }
        void add(const std::string& name, size_t start_at) {
            nr_names_below_ ++;
            if (start_at >= name.size()) return;
            char next_char = name[start_at];
            node* next_node = links_[next_char-'a'];
            if (next_node == NULL) {
                next_node = links_[next_char-'a'] = new node(name.substr(0,(start_at+1)));
            }
            next_node->add(name, start_at+1);
            return;
        }
        size_t get_nr_names_below() { 
            return nr_names_below_;
        }
    };
    node root_; 
public:
    void push_back(const std::string& name) {
        root_.add(name, 0);
    }
    size_t get_num_matches(const std::string& prefix) {
        node* curr_node = &root_;
        size_t curr_position = 0;
        char curr_char = prefix[curr_position];
        while (curr_position < prefix.size()) {
            curr_char = prefix[curr_position];
            curr_node = curr_node->links_[curr_char-'a'];
            if (curr_node == NULL) {
                return 0;
            }
            curr_position ++;
        }
        assert(curr_node != NULL);
        return curr_node->get_nr_names_below();
    }
    trie() : root_("") {}
};

class Directory {
    //std::vector<std::string> names_;
    trie names_;
    public:
    Directory() {}
    void add(const std::string& name) 
    {
        names_.push_back(name);   
    }
    /*
    size_t get_num_matches_vector(const std::string& prefix)
    {
        size_t nr_matches = 0;
        for (auto& name: names_) {
            if (prefix == name.substr(0,prefix.size())) {
                nr_matches ++;
            }
        }
        return nr_matches;
    }
    */
    size_t get_num_matches(const std::string& prefix) 
    {
        return names_.get_num_matches(prefix);
    }
};

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Directory directory;
    
    for (int n_itr = 0; n_itr < n; n_itr++) {
        string opContact_temp;
        getline(cin, opContact_temp);

        vector<string> opContact = split_string(opContact_temp);

        string op = opContact[0];

        string contact = opContact[1];
        
        if (op == "add") {
            directory.add(contact);
        } else if (op == "find") {
            cout << directory.get_num_matches(contact) << std::endl;
        } else {
            assert(false);
        }
    }

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

