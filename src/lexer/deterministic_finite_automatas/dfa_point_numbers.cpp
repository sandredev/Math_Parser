#include "../../../include/lexer/deterministic_finite_automatas/dfa_point_numbers.hpp"
#include <string>
#include <iostream>
#include <cctype>
#include <stdexcept>

lexer::DFAPointNumbers::DFAPointNumbers() {
    /*
        DFA:
                    digit       +/-     e/E     point       f/F         l/L
        init        ds          -       -       p           -           -               0
        ds          ds          -       a_e     ds_p/p_ds   -           -               0
        p           p_ds/p_ds   -       -       -           -           -               0
        a_e         a_e_ds      a_e_sg  -       -           -           -               0
        a_e_sg      a_e_ds      -       -       -           -           -               0
        a_e_ds      a_e_ds      -       -       -           float       long_double     1
        ds_p/p_ds   p_ds/p_ds   -       a_e     -           float       long_double     1
        float       -           -       -       -           -           -               1
        long_double -           -       -       -           -           -               1

        Notes:
        -  d means digit
        -  ds means digit sequence
        -  a means after
        -  p means point
        -  e means exponent ("e/E")
        -  float is an state where the current lexem is a float number
        -  long_double is an state where the current lexem is a long double number
        -  if an state is an acceptance state and it's neither float nor long_double state,
            then the state corresponds to a double number
        -  in the DFA implementation -1 means no transition
    */
    transitions_table = {
        { 1, -1, -1,  2, -1, -1, 0},  // init
        { 1, -1,  3,  6, -1, -1, 0},  // ds
        { 6, -1, -1, -1, -1, -1, 0},  // p
        { 5,  4, -1, -1, -1, -1, 0},  // a_e
        { 5, -1, -1, -1, -1, -1, 0},  // a_e_sg
        { 5, -1, -1, -1,  7,  8, 1},  // a_e_ds
        { 6, -1,  3, -1,  7,  8, 1},  // ds_p/p_ds
        {-1, -1, -1, -1, -1, -1, 1},  // float
        {-1, -1, -1, -1, -1, -1, 1}   // long_double
    };
    current_state = 0;
};

void lexer::DFAPointNumbers::do_transition(char& input) {
    if (current_state == -1) return;
    if (std::isdigit(input)) current_state = transitions_table[current_state][0];
    else if (input == '+' || input == '-') current_state = transitions_table[current_state][1];
    else if (input == 'e' || input == 'E') current_state = transitions_table[current_state][2];
    else if (input == '.') current_state = transitions_table[current_state][3];
    else if (input == 'f' || input == 'F') current_state = transitions_table[current_state][4];
    else if (input == 'l' || input == 'L') current_state = transitions_table[current_state][5];
    else current_state = -1;
};

void lexer::DFAPointNumbers::test() {
    std::cout << "Currently testing DFA that recognizes point numbers..." << std::endl;
    std::vector<std::string> test_inputs = {
        // Doubles
        "0.", ".0", "1.0", "123.456",
        "0.0e0", "1e1", "1e+1", "1e-1",
        "1.2e3", "12.34e-56", "0e0",
        ".1e2", ".123E3", "123.E4",
        "12.34e+24",
        // Floats
        "0.f", ".0f", "1.0f", "123.456f",
        "1e1f", "1e+1f", "1e-1f",
        "1.2e3f", "12.34e-56f", ".1e2f",
        "1.F", ".1E3F",
        // Long doubles
        "0.l", ".0l", "1.0l", "123.456l",
        "1e1l", "1e+1l", "1e-1l",
        "1.2e3l", "12.34e-56l", ".1e2l",
        "1.L", ".1E3L",
        // Invalid
        ".", "e1", "1e", "1e+", "1e-",
        ".e1", "1.2.3", "1ee2", "1e1.2",
        "f1", "1lf", "1fe1", "1el", "1.2ff", "1.2fl"
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

std::string lexer::DFAPointNumbers::which_token_is() {
    switch (current_state)
    {
        case 5:
        case 6:
            return "DOUBLE_NUM";
            break;
        case 7:
            return "FLOAT_NUM";
            break;
        case 8:
            return "LONG_DOUBLE_NUM";
            break;
        default:
            return "NONE";
    }
}

bool lexer::DFAPointNumbers::is_lexem() const {
    if (current_state == -1) return false;
    return transitions_table[current_state][6] == 1;
}