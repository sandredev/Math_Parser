#include "../../../include/lexer/deterministic_finite_automatas/dfa_identifiers.hpp"
#include <string>
#include <vector>
#include <cctype>
#include <iostream>

lexer::DFAIdentifiers::DFAIdentifiers() {
    /*
    DFA:
            letter/_   digit
    init    valid      -        0
    valid   valid      valid    1

    the last column specifies whether the state is acceptance or not.

    -1 means no transition.
    */
    transitions_table = {{1, -1, 0}, // init
                         {1, 1, 1}}; // valid
};

void lexer::DFAIdentifiers::do_transition(char& input) {
    if (current_state == -1) return;
    else if (std::isalpha(input) || input == '_') {
        current_state = transitions_table[current_state][0];
    }
    else if (std::isdigit(input)) {
        current_state = transitions_table[current_state][1];
    }
    else {
        current_state = -1;
    }

};

void lexer::DFAIdentifiers::test() {
    std::cout << "Currently testing DFA that recognizes C identifiers..." << std::endl;
    std::vector<std::string> test_inputs = {
        "a",        
        "_var",     
        "abc123",   
        "_123",     
        "a_b_c",    
        "9abc",     
        "123",      
        "var!",     
        "",         
        "____",     
        "a9_",      
        "a9-1"   
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

bool lexer::DFAIdentifiers::is_lexem() const {
    if (current_state == -1) return false;
    return transitions_table[current_state][2] == 1;
}

std::string lexer::DFAIdentifiers::which_token_is() {
    if (is_lexem()) return "ID";
    return "NONE";
}