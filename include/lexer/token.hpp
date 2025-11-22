#pragma once
#include <string>

namespace lexer {
    class Token {
        private:
            std::string value;
            std::string type;
        public:
            Token(std::string value, std::string type);

            std::string get_value() const;
            std::string get_type() const;
    };
}