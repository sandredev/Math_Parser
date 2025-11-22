#include "../../../include/lexer/deterministic_finite_automatas/dfa_math_symbols.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

lexer::DFAMathSymbols::DFAMathSymbols() {
    /*
    DFA:
                    +                -                *                /                %           ^  
    init            ADD              SUBSTRACT        MULIIPLY         DIVIDE           MOD         POW     0 
    PLUS            -                -                -                -                -           -       1
    MINUS           -                -                -                -                -           -       1
    MUL_OP          -                -                -                -                -           -       1        
    DIV_OP          -                -                -                -                -           -       1 
    MOD_OP          -                -                -                -                -           -       1
    POW_OP          -                -                -                -                -           -       1
    the last column specifies whether the state is acceptance or not.

    -1 means no transition.
    */
    transitions_table = {
        {1, 2, 3, 4, 5, 6, 0}, // init
        {-1, -1, -1, -1, -1, -1, 1}, // PLUS
        {-1, -1, -1, -1, -1, -1, 1}, // MINUS
        {-1, -1, -1, -1, -1, -1, 1}, // MUL_OP
        {-1, -1, -1, -1, -1, -1, 1}, // DIV_OP
        {-1, -1, -1, -1, -1, -1, 1}, // MOD_OP
        {-1, -1, -1, -1, -1, -1, 1}  // POW_OP
    };
};

void lexer::DFAMathSymbols::do_transition(char& input) {
    if (current_state == -1) return;
    switch (input)
    {
    case '+':
        current_state = transitions_table[current_state][0];
        break;
    case '-':
        current_state = transitions_table[current_state][1];
        break;
    case '*':
        current_state = transitions_table[current_state][2];
        break;
    case '/':
        current_state = transitions_table[current_state][3];
        break;
    case '%':
        current_state = transitions_table[current_state][4];
        break;
    case '^':
        current_state = transitions_table[current_state][5];
        break;
    default:
        current_state = -1;
        break;
    }
};

void lexer::DFAMathSymbols::test() {
    std::cout << "Currently testing DFA that recognizes unique math symbols..." << std::endl;
    std::vector<std::string> test_inputs = {
        "+",
        "-",
        "*",
        "/",
        "%",
        "^",
        "sadas",
        "!#$",
        "++",
        "--",
        "//",
        "**",
        "%",
        "-%*"
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

std::string lexer::DFAMathSymbols::which_token_is() {
    switch (current_state)
    {
    case 1:
        return "PLUS";
        break;
    case 2:
        return "MINUS";
        break;
    case 3:
        return "MUL_OP";
        break;
    case 4:
        return "DIV_OP";
        break;
    case 5:
        return "MOD_OP";
        break;
    case 6:
        return "POW_OP";
        break;
    default:
        return "NONE";
        break;
    }
}

bool lexer::DFAMathSymbols::is_lexem() const {
    if (current_state == -1) return false;
    return transitions_table[current_state][6] == 1;
}