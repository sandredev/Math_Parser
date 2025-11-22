#include "../../../include/lexer/deterministic_finite_automatas/dfa.hpp"

lexer::DFA::DFA() : current_state(0) {};

lexer::DFA::~DFA() = default;

void lexer::DFA::restart() {
    current_state = 0;
}

int lexer::DFA::get_current_state() {
    return current_state;
}
