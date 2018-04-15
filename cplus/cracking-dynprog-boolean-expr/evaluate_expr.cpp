#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>


#if 0
expr = 1|0^1&0^1 == 1
       nr_ways("1",1) x nr_ways("0^1&0^1", 2)

a$b$c$d
=>
(a)$(b$c$d)
==>
(a)$((b)$(c$d))

(a$b) === can be done in only N ways, where N=1
#endif


int get_nr_operators(const std::string& expr)
{
    const char* operators = "^&|"; // accepted binary operators
    int nr_operators = 0;
    size_t found_at = expr.find_first_of(operators);
    while (found_at != std::string::npos) {
        nr_operators ++;
        found_at = expr.find_first_of(operators, found_at+1);
    }
    return nr_operators;
}

bool split_expression_at(const std::string& expr, int split_at, std::string& left, std::string& right, char& the_operator)
{
    const char* operators = "^&|"; // accepted binary operators
    int nr_operators = 0;
    size_t found_at = expr.find_first_of(operators);
    while (found_at != std::string::npos && nr_operators < split_at) {
        nr_operators ++;
        found_at = expr.find_first_of(operators, found_at+1);
    }

    if (nr_operators == split_at && found_at != std::string::npos) {
        left = expr.substr(0,found_at);
        right = expr.substr(found_at+1);
        the_operator = expr[found_at];
        return true;
    } else {
        return false;
    }
}

int nr_ways_this_can_be_evaluated(const std::string& expr, bool evaluate_to)
{
    int nr_operators = get_nr_operators(expr);
    if (nr_operators < 1) {
        int rc = -1;
        assert(expr.size() == 1);
        if (evaluate_to == true) {
            if (expr[0] == '1') {
                rc = 1;
            } else {
                rc = 0;
            }
        } else {
            if (expr[0] == '0') {
                rc = 1;
            } else {
                rc = 0;
            }
        }
std::cout << "Expr: " << expr << " to " << evaluate_to << " can be evaluated in " << rc << " ways\n";
        return rc;
    }
#if 0
 else if (nr_operators == 2) {
std::cout << "expr: " << expr << " can be evaluated in " << 2 << " ways\n";
        return 2;
    }
#endif
    
    int nr_ways = 0;
    int nr_combos = 0;
    for (int split_at=0; split_at<nr_operators; split_at++) {
        std::string left, right;
        char the_operator = '\0';
        assert(split_expression_at(expr, split_at, left, right, the_operator));
std::cout << "Splitting " << expr << " into " << left << " and " << right << " by operator " << the_operator << std::endl;
        int nr_left_true = nr_ways_this_can_be_evaluated(left, true);
        int nr_left_false = nr_ways_this_can_be_evaluated(left, false);
        int nr_right_true = nr_ways_this_can_be_evaluated(right, true);
        int nr_right_false = nr_ways_this_can_be_evaluated(right, false);
        switch (the_operator) {
        case '&':
            if (evaluate_to == true) {
                //both sides must be true
                nr_combos = nr_left_true * nr_right_true;
            } else {
                nr_combos = (nr_left_true+nr_left_false) * (nr_right_true+nr_right_false) -
                                (nr_left_true*nr_right_true);
            }
            break;
        case '|':
            if (evaluate_to == true) {
                //any side can be true
                nr_combos = (nr_left_true+nr_left_false) * (nr_right_true+nr_right_false) -
                                (nr_left_false*nr_right_false);
            } else {
                nr_combos = nr_left_false * nr_right_false;
            }
            break;
        case '^':
            if (evaluate_to == true) {
                nr_combos = (nr_left_true * nr_right_false) + (nr_left_false * nr_right_true);
            } else {
                nr_combos = (nr_left_true * nr_right_true) + (nr_left_false * nr_right_false);
            }
            break;
        default: assert(false);
        }
        nr_ways += nr_combos;
    }
    std::cout << "EXpr: " << expr << " to " << evaluate_to << " can be evaluated in " << nr_ways << " ways\n";
    return nr_ways;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <valid expression>" << std::endl;
        return 1;
    }
    std::string expr(argv[1]);
    std::cout << "Expression: " << expr << " can be evaluated to true  in " << nr_ways_this_can_be_evaluated(expr, true) << " ways." << std::endl;
    std::cout << "Expression: " << expr << " can be evaluated to false in " << nr_ways_this_can_be_evaluated(expr, false) << " ways." << std::endl;
    return 0;
}


