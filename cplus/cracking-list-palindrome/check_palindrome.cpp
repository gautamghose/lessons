#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <stack>


/// One permutation is captured in a vector
/// height, weight
typedef std::pair<int,int> building_block_t;

struct XGreater {
    bool operator() (const building_block_t& left, const building_block_t& right) const
    {
        std::cout << "XGreater compare()\n";
        return (left.second < right.second);
    }
};

bool compare_operator (const building_block_t& left, const building_block_t& right)
{
    std::cout << "operator< compare()\n";
    return (left.first < right.first);
}

void sort_all_by_height(std::vector<building_block_t>& all_blocks)
{
#if 1
    std::sort(all_blocks.begin(), all_blocks.end(), [] (const building_block_t& left, const building_block_t& right) 
                                                    {
                                                        std::cout << "lambda compare()\n";
                                                        return (left.first > right.first);
                                                    });
#else
    std::sort(all_blocks.begin(), all_blocks.end(), XGreater());
    std::sort(all_blocks.begin(), all_blocks.end(), compare_operator);
#endif

    
}

struct node {
    node* next;
    char c;
    node(char in) : next(NULL), c(in) {}
};

node* create_slist(std::string input)
{
    node* head = NULL;
    node* tail = NULL;
    for (char c: input) {
        if (!head) {
            head = new node(c);
            tail = head;
        } else {
            tail->next = new node(c);
            tail = tail->next;
        }
    }
    return head;
}

int compute_slist_len(node* head)
{
    int len = 0;
    while (head) {
        len ++;
        head = head->next;
    }
    return len;
}

struct result {
    node* endpoint;
    bool is_palin;
    result(node* endp, bool palin) : endpoint(endp), is_palin(palin) {}
};

result check_palindrome_recurse(node* head, int len)
{
    if (len == 0 || head == NULL) {
        return result(head, true);
    }
    if (len == 1) {
        return result(head, true);
    } else if (len == 2) {
        return result(head->next, (head->c == head->next->c));
    }
    result res = check_palindrome_recurse(head->next, len-2);
    result combined(NULL, false);
    if (res.endpoint) {
        combined.endpoint = res.endpoint->next;
    }
    if (res.is_palin) {
        if (combined.endpoint) {
            combined.is_palin = (combined.endpoint->c == head->c);
        }
    }
    return combined;
}

bool check_palindrome_rec(node* head)
{
    int len = compute_slist_len(head);
    result r = check_palindrome_recurse(head, len);
    return r.is_palin;
}

bool check_palindrome_stack(node* head)
{
    node *slow = head;
    node *fast = head;
    std::stack<char> helper;
    bool ignore_top = false;
    while (fast != NULL) {
        helper.push(slow->c);
        slow = slow->next;
        fast = fast->next;
        if (fast) {
            fast = fast->next;
        } else {
            ignore_top = true;
        }
    }

    //wherever slow is pointing to, just move on.
    if (ignore_top) {
        helper.pop();
    }
    //std::cout << "slow is at " << slow->c << " and ignore_top was " << ignore_top << std::endl;
    while (slow != NULL) {
        if (helper.top() != slow->c) 
            return false;
        helper.pop();
        slow = slow->next;
    }
    return true;
}

bool check_palindrome(node* head)
{
    return check_palindrome_rec(head);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Give the input stringin arg[1]\n";
        return 1;
    }
    std::cout << argv[1] << ": is" << (check_palindrome(create_slist(std::string(argv[1]))) ? "" : " not") << " a palindrome.\n";
    return 0;
}


