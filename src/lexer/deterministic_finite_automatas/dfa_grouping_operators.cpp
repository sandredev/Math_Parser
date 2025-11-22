#include "../../../include/lexer/deterministic_finite_automatas/dfa_grouping_operators.hpp"
#include <string>
#include <vector>
#include <iostream>


lexer::DFAGroupingOperators::DFAGroupingOperators() {
    /*
    DFA:
                (           )           [           ]           {           }
    init        LPAR        RPAR        LSQUARE     RSQUARE     LBRACE      RBRACE     0
    LPAR        -           -           -           -           -           -           1
    RPAR        -           -           -           -           -           -           1
    LSQUARE     -           -           -           -           -           -           1
    RSQUARE     -           -           -           -           -           -           1
    LBRACE      -           -           -           -           -           -           1     
    RBRACE      -           -           -           -           -           -           1
    
    the last column specifies whether the state is acceptance or not.

    -1 means no transition.
    */
    transitions_table = {
        {1, 2, 3, 4, 5, 6, 0}, // init
        {-1, -1, -1, -1, -1, -1, 1}, // LPAR
        {-1, -1, -1, -1, -1, -1, 1}, // RPAR
        {-1, -1, -1, -1, -1, -1, 1}, // LSQUARE
        {-1, -1, -1, -1, -1, -1, 1}, // RSQUARE
        {-1, -1, -1, -1, -1, -1, 1}, // LBRACE
        {-1, -1, -1, -1, -1, -1, 1}  // RBRACE
    };
};

void lexer::DFAGroupingOperators::do_transition(char& input) {
    if (current_state == -1) return;
    switch (input) 
    {
        case '(': 
            current_state = transitions_table[current_state][0];
            break;
        case ')':
            current_state = transitions_table[current_state][1];
            break;
        case '[':
            current_state = transitions_table[current_state][2];
            break;
        case ']':
            current_state = transitions_table[current_state][3];
            break;
        case '{':
            current_state = transitions_table[current_state][4];
            break;
        case '}':
            current_state = transitions_table[current_state][5];
            break;
        default:
            current_state = -1;
    }
};

void lexer::DFAGroupingOperators::test() {
    std::cout << "Currently testing DFA that recognizes grouping operators..." << std::endl;
    std::vector<std::string> test_inputs = {
        "(",
        ")",
        "{",
        "}",
        "[",
        "]",
        "()",
        "[]",
        "{}"
    };
    for (std::string& input : test_inputs) {
        current_state = 0;
        for (char c : input) {
            do_transition(c);
        }
        std::cout << input << " is lexem: " << std::boolalpha << is_lexem();
        std::cout << "; TOKEN: " << which_token_is() << std::endl;
    }
    current_state = 0;
};

bool lexer::DFAGroupingOperators::is_lexem() const {
    if (current_state == -1) return false;
    return transitions_table[current_state][6] == 1;
}

std::string lexer::DFAGroupingOperators::which_token_is() {
    switch (current_state)
    {
        case 1:
            return "LPAR";
            break;
        case 2:
            return "RPAR";
            break;
        case 3:
            return "LSQUARE";
            break;
        case 4:
            return "RSQUARE";
            break;
        case 5:
            return "LBRACE";
            break;
        case 6:
            return "RBRACE";
            break;
        default:
            return "NONE";
            break;
    }
}