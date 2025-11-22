#include "../../../include/lexer/deterministic_finite_automatas/dfa_integers.hpp"
#include <string>
#include <vector>
#include <cctype>
#include <iostream>


lexer::DFAIntegers::DFAIntegers() {
    /*
    DFA:
            digit
    init    valid    0
    valid   valid    1

    the last column specifies whether the state is acceptance or not.

    -1 means no transition.
    */
    transitions_table = {{1, 0},
                         {1, 1}};
};

void lexer::DFAIntegers::do_transition(char& input) {
    if (current_state == -1) return;
    else if (std::isdigit(input)) {
        current_state = transitions_table[current_state][0];
    }
    else {
        current_state = -1;
    }
};

void lexer::DFAIntegers::test() {
    std::cout << "Currently testing DFA that recognizes integers..." << std::endl;
    std::vector<std::string> test_inputs = {
        "0",         
        "5",         
        "123",      
        "999999",   
        "",          
        "a",          
        "12a",        
        "3.14",      
        "-12",        
        " 42",        
        "42 ",       
        "4 2",        
        "12\n",       
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

bool lexer::DFAIntegers::is_lexem() const {
    if (current_state == -1) return false;
    return transitions_table[current_state][1] == 1;
}


std::string lexer::DFAIntegers::which_token_is() {
    if (is_lexem()) return "INT_NUM";
    return "NONE";
}