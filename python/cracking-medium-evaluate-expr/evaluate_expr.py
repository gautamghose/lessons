#!/usr/bin/python

import sys
import re #regular expression library to split a string on multiple delimiters

class eOperator(object):
    """Enumeration
    """
    eAdd = 1
    eSub = 2
    eMul = 3
    eDiv = 4

def evaluate_L2R_expression(literals, operators):
    """Evaluate a sanitized expression
    Evaluate an expression expressed in terms of literals and operators.
    Args:
        literals: list, a list of integers
        operators: list of only eAdd and eSub enums
    Returns:
        an integer which is the evaluation of the expression
    """
    if len(literals) == 1:
        return literals[0]

    if len(literals) == 2:
        op = operators[0]
        if op == eOperator.eAdd:
            return (literals[0] + literals[1])
        elif op == eOperator.eSub:
            return (literals[0] - literals[1])
        elif op == eOperator.eMul:
            return (literals[0] * literals[1])
        elif op == eOperator.eDiv:
            return (literals[0] / literals[1])
        else:
            assert(False)

    curr = literals[0]
    op_idx = 0
    while op_idx < len(operators):
        two_literals = [curr, literals[op_idx+1]]
        one_operator = [operators[op_idx]]
        curr = evaluate_L2R_expression(two_literals, one_operator)
        op_idx += 1
    return curr
    
def find_next_operator(operators, search_for):
    """Search for one of the operators in the operators list
    Args:
        operators: list, of eOperator enums
        search_for: list, of eOperator enums
    Return:
        integer, the index of first operator in the operators list that matches 
                 (-1) if nothing matches
    """
    idx = 0
    matched = False
    for o in operators:
        if o in search_for: 
            matched = True
            break
        idx += 1
    if matched:
        return idx
    else:
        return (-1)
    
def evaluate_expression(literals, operators):
    """Evaluate a sanitized expression
    Evaluate an expression expressed in terms of literals and operators.
    Args:
        literals: list, a list of integers
        operators: list of eOperator enums
    Returns:
        an integer which is the evaluation of the expression
    """
    assert(len(literals) == len(operators)+1)

    if len(literals) <= 2:
        return evaluate_L2R_expression(literals, operators)

    mod_literals = literals
    mod_operators = operators
    while len(mod_literals) > 1:
        idx = find_next_operator(mod_operators, [eOperator.eMul, eOperator.eDiv])
        if idx == -1:
            final = evaluate_L2R_expression(mod_literals, mod_operators)
            mod_literals = [final]
            mod_operators = []
        else:
            subset_expr_literals = [mod_literals[idx], mod_literals[idx+1]]
            subset_expr_operators = [mod_operators[idx]]
            semifinal = evaluate_L2R_expression(subset_expr_literals, subset_expr_operators)
            mod_mod_literals = mod_literals[:idx]
            mod_mod_literals += [semifinal]
            mod_mod_literals += mod_literals[idx+2:]
            assert(len(mod_mod_literals) == len(mod_literals)-1)
            mod_literals = mod_mod_literals
            mod_mod_operators = mod_operators[:idx]
            mod_mod_operators += mod_operators[idx+1:]
            assert(len(mod_mod_operators) == len(mod_operators)-1)
            mod_operators = mod_mod_operators
    
    assert(len(mod_literals) == 1)
    return mod_literals[0]

def main():
  if len(sys.argv) < 2:
    print "Usage:", sys.argv[0], "<Expression-to-evaluate>"
    print "Example:", sys.argv[0], "\"2+5*3-10\""
    print "The answer should be printed as: 7"
    return 0

  expr_str = sys.argv[1].strip()
  #expr_literals_str = expr_str.split('+-*/')
  expr_literals_str = re.split('\+|\-|\*|\/', expr_str)
  expr_literals = []
  for lit in expr_literals_str:
    expr_literals.append(int(lit))

  expr_operators = []
  for c in expr_str:
    if c in '+-*/':
      if c == '+':
        expr_operators.append(eOperator.eAdd)
      elif c == '-':
        expr_operators.append(eOperator.eSub)
      elif c == '*':
        expr_operators.append(eOperator.eMul)
      elif c == '/':
        expr_operators.append(eOperator.eDiv)
      else:
        assert(False)

  if len(expr_literals) != len(expr_operators)+1:
    print "Invalid expr:", expr_str
    return 1

  result = evaluate_expression(expr_literals, expr_operators)
  print "Result:", result

if __name__ == "__main__":
  main()

