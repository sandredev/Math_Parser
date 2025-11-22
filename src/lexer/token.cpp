#include "../../include/lexer/token.hpp"
#include <string>


lexer::Token::Token(std::string value, std::string type) : value(value), type(type) {}
std::string lexer::Token::get_value() const {
    return value;
}
std::string lexer::Token::get_type() const {
    return type;
}
