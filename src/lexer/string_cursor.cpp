#include "../../include/lexer/string_cursor.hpp"
#include <optional>

lexer::StringCursor::StringCursor(const std::string& content) : content(content), position(0) {};

std::optional<char> lexer::StringCursor::peek() const {
    if (position < content.size()) {
        return content[position];
    }
    return std::nullopt; // End of string
}

std::optional<char> lexer::StringCursor::get_next_char() {
    if (position < content.size()) {
        position++;
        return content[position];
    }
    return std::nullopt; 
}

bool lexer::StringCursor::is_end() const {
    return position >= content.size();

}
void lexer::StringCursor::reset() {
    position = 0;
}
