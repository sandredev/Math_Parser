#include "../include/variable_environment.hpp"
#include <unordered_map>
#include <string>
#include <optional>

bool VariableEnvironment::register_variable(const std::string& name, double value) {
    if (exists(name)) return false;
    table[name] = value;
    return true;
}

bool VariableEnvironment::set_variable(const std::string& name, double value) {
    if (!exists(name)) return false;
    table[name] = value;
    return true;
}

std::optional<double> VariableEnvironment::get_variable(const std::string& name) const {
    /*
    Using this logic instead of just 'return table[name]' is more efficient
    and secure.
    */
    auto it = table.find(name);
    if (it == table.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool VariableEnvironment::exists(const std::string& name) const {
    return table.find(name) != table.end();
}

void VariableEnvironment::clear() {
    table.clear();
}