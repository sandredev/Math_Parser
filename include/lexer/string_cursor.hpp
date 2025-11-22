#pragma once

#include <string>
#include <optional>
namespace lexer {
    class StringCursor {
        private:
            std::string content;
            size_t position;
        public:
            StringCursor(const std::string& str);

            // Returns the current character without advancing the cursor
            std::optional<char> peek() const;
            
            // Advances the cursor and returns the current character
            std::optional<char> get_next_char();

            // Checks if the cursor has reached the end of the string
            bool is_end() const;
            
            // Resets the cursor to the beginning of the string
            void reset();
    };
}