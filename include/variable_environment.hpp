#pragma once
#include <unordered_map>
#include <string>
#include <optional>

class VariableEnvironment {
private:
    std::unordered_map<std::string, double> table;

public:
    // Updates only if exists
    bool register_variable(const std::string& name, double value);

    // Set only if exists
    bool set_variable(const std::string& name, double value);

    // Get a value only if variable exists
    std::optional<double> get_variable(const std::string& name) const;

    // Check if there's already a variable
    bool exists(const std::string& name) const;

    void clear();
};