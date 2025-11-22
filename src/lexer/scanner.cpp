#include "../../include/lexer/scanner.hpp"
#include "../../include/lexer/deterministic_finite_automatas/dfa_identifiers.hpp"
#include "../../include/lexer/deterministic_finite_automatas/dfa_integers.hpp"
#include "../../include/lexer/deterministic_finite_automatas/dfa_math_symbols.hpp"
#include "../../include/lexer/deterministic_finite_automatas/dfa_point_numbers.hpp"
#include "../../include/lexer/deterministic_finite_automatas/dfa_grouping_operators.hpp"
#include "../../include/lexer/string_cursor.hpp"
#include "../../include/lexer/token.hpp"
#include <string>
#include <iostream>

lexer::Scanner::Scanner(std::string input) : prefix(""), string_cursor(input), input(input) { }; 

lexer::Token lexer::Scanner::get_next_token() {
    if (is_end_of_input()) return Token("", "NONE"); // This should throw an exception instead
    last_detected_token_type = "NONE";
    prefix = string_cursor.peek().value();
    restart_dfas();
    make_dfas_do_transition(string_cursor.peek().value());
    update_last_detected_token_if_not_keyword();
    update_last_detected_token_if_keyword(); 
    while (is_current_prefix_a_lexem() || is_a_longer_prefix_possible()) { 
        string_cursor.get_next_char();
        if (is_end_of_input()) break;
        prefix += string_cursor.peek().value();
        make_dfas_do_transition(string_cursor.peek().value());
        update_last_detected_token_if_not_keyword();
        update_last_detected_token_if_keyword();
    }
    
    // Skip delimiters
    if (!is_end_of_input()) {
        while (string_cursor.peek().value() == '\n' || string_cursor.peek().value() == '\t' || string_cursor.peek().value() == ' ') {
            string_cursor.get_next_char();
        }
        prefix = prefix.substr(0, prefix.length() - 1);
    }

    return Token(prefix, last_detected_token_type); // 
}

// The main scanning function that processes the entire input
//
// Not usable for this project but useful for debugging the scanner
void lexer::Scanner::scan() {
    last_detected_token_type = "NONE";
    std::cout << "Currently scanning code..." << std::endl << input << std::endl;
    input.push_back('\0'); // This character is appended to ensure the last token of the input is processed
    for (char c : input) {
        // If the current character is a character that separates tokens then outputs the
        // last token detected before that character and continues scanning
        if (c == '\n' || c == '\t' || c == '\0' || c == ' ') {
            if (last_detected_token_type != "NONE") std::cout << "TOKEN: " << last_detected_token_type 
                                                << " \"" << prefix << "\"" << std::endl;
            prefix = "";
            restart_dfas();
            last_detected_token_type = "NONE";
            continue;
        }
        prefix += c;
        make_dfas_do_transition(c);
        update_last_detected_token_if_not_keyword();
        update_last_detected_token_if_keyword();

        // If the current prefix with the last character added is not a lexem of any DFA anymore, 
        // outputs the last token detected and the prefix before it stopped beign a lexem. 
        // Then restarts the value of the prefix starting with the new character that made the last
        // prefix stop beign a lexem to scan a new token.
        if (!is_current_prefix_a_lexem() && !is_a_longer_prefix_possible()) {
            std::cout << "TOKEN: " << last_detected_token_type << " \"" << prefix.substr(0, prefix.length() - 1) << "\"" << std::endl;
            prefix = c;
            last_detected_token_type = "NONE";
            restart_dfas();
            update_last_detected_token_if_keyword();
            make_dfas_do_transition(c);
            update_last_detected_token_if_not_keyword();
        }
    }
}

bool lexer::Scanner::update_last_detected_token_if_keyword() {
    if (prefix == "sin") {
        last_detected_token_type = "SIN";
        return true;
    }
    if (prefix == "cos") {
        last_detected_token_type = "COS";
        return true;
    }
    if (prefix == "tan") {
        last_detected_token_type = "TAN";
        return true;
    }
    if (prefix == "sen") {
        last_detected_token_type = "SEN";
        return true;
    }
    if (prefix == "pi") {
        last_detected_token_type = "PI";
        return true;
    }
    if (prefix == "e") {
        last_detected_token_type = "E";
        return true;
    }
    return false;
};

void lexer::Scanner::update_last_detected_token_if_not_keyword() {
    last_detected_token_type = (dfa_identifiers.which_token_is() != "NONE") ? 
                    dfa_identifiers.which_token_is() : last_detected_token_type;
    last_detected_token_type = (dfa_point_numbers.which_token_is() != "NONE") ? 
                    dfa_point_numbers.which_token_is() : last_detected_token_type;
    last_detected_token_type = (dfa_integers.which_token_is() != "NONE") ? 
                    dfa_integers.which_token_is() : last_detected_token_type; 
    last_detected_token_type = (dfa_grouping_operators.which_token_is() != "NONE") ? 
                    dfa_grouping_operators.which_token_is() : last_detected_token_type;
    last_detected_token_type = (dfa_math_symbols.which_token_is() != "NONE") ? 
                    dfa_math_symbols.which_token_is() : last_detected_token_type;       
}

void lexer::Scanner::make_dfas_do_transition(char current_character) {
    dfa_identifiers.do_transition(current_character);
    dfa_integers.do_transition(current_character);
    dfa_math_symbols.do_transition(current_character);
    dfa_point_numbers.do_transition(current_character);
    dfa_grouping_operators.do_transition(current_character);
};

void lexer::Scanner::restart_dfas() {
    dfa_identifiers.restart();
    dfa_integers.restart();
    dfa_math_symbols.restart();
    dfa_point_numbers.restart();
    dfa_grouping_operators.restart();
};

bool lexer::Scanner::is_current_prefix_a_lexem() {
    if (dfa_grouping_operators.is_lexem() || dfa_identifiers.is_lexem() ||
        dfa_integers.is_lexem() || dfa_math_symbols.is_lexem() || dfa_point_numbers.is_lexem()  ||
        prefix == "sin" || prefix == "cos" || prefix == "tan" || prefix == "sen") {
        return true;
    }
    return false;
}

bool lexer::Scanner::is_a_longer_prefix_possible() {
    // The prefix can be a point number
    bool is_possible = (dfa_point_numbers.get_current_state() == 2 ||
                        dfa_point_numbers.get_current_state() == 3 ||
                        dfa_point_numbers.get_current_state() == 4);
    return is_possible;
}

bool lexer::Scanner::is_end_of_input() {
    return string_cursor.is_end();
}

lexer::Token lexer::Scanner::get_current_token() {
    return Token(prefix, last_detected_token_type);
}

void lexer::Scanner::restart_cursor() {
    string_cursor.reset();
}

void lexer::Scanner::change_input(std::string new_input) {
    input = new_input;
    restart_dfas();
    restart_cursor();
    last_detected_token_type = "NONE";
    prefix = "";
}