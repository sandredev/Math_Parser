#include "../../include/parser/parser.hpp"
#include "../../include/lexer/scanner.hpp"
#include <iostream>
#include <string>
#include <cmath>

parser::Parser::Parser(lexer::Scanner& scanner) : scanner(scanner) {};

double parser::Parser::parse() {
    if (ast.get_root()) {
        return ast.evaluate();
    }
    current_token_type = scanner.get_next_token().get_type();
    ast.set_root(math_expression()); 
    if (current_token_type != "NONE") {
        std::cout << "Error: Unexpected token '" << current_token_type << "' at the end of expression." << std::endl;
    }
    std::cout << "AST generated: " << ast.to_string();
    return ast.evaluate();
}

// <math_expression> ::= <mult/div_expression> <sum/substract_list>
parser::ASTNode* parser::Parser::math_expression() {
    ASTNode* left = mult_div_expression();
    return sum_substract_list(left);
}

/*
<sum/substract_list> ::= + <mult/div_expression> <sum/substract_list> 
                       | - <mult/div_expression> <sum/substract_list>
                       | ε
*/
parser::ASTNode* parser::Parser::sum_substract_list(ASTNode* left) {
    if (current_token_type == "PLUS") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where PLUS should be parsed " <<std::endl;
        current_token_type = scanner.get_next_token().get_type();
        ASTNode* right = mult_div_expression();
        BinaryOperatorNode* parent = new BinaryOperatorNode("+");
        parent->set_left_operand(left);
        parent->set_right_operand(right);
        return sum_substract_list(parent);
    } else if (current_token_type == "MINUS") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where MINUS should be parsed " << std::endl;
        current_token_type = scanner.get_next_token().get_type();
        ASTNode* right = mult_div_expression();
        BinaryOperatorNode* parent = new BinaryOperatorNode("-");
        parent->set_left_operand(left);
        parent->set_right_operand(right);
        return sum_substract_list(parent);
    } else {
        return left;
        // ε production, do nothing
    }
}

//<mult/div_expression> ::= <unary_expression> <mult/div_list>
parser::ASTNode* parser::Parser::mult_div_expression() {
    ASTNode* left = unary_expression();
    return mult_div_list(left);
}

/*
<mult/div_list> ::= * <unary_expression> <mult/div_list>
                  | / <unary_expression> <mult/div_list>
                  | ε
*/
parser::ASTNode* parser::Parser::mult_div_list(ASTNode* left) {
    if (current_token_type == "MUL_OP") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where MUL_OP should be parsed "<< std::endl;
        current_token_type = scanner.get_next_token().get_type();
        ASTNode* right = unary_expression();
        BinaryOperatorNode* parent = new BinaryOperatorNode("*");
        parent->set_left_operand(left);
        parent->set_right_operand(right);
        return mult_div_list(parent);
    } else if (current_token_type == "DIV_OP") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where DIV_OP should be parsed " << std::endl;
        current_token_type = scanner.get_next_token().get_type();
        ASTNode* right = unary_expression();
        BinaryOperatorNode* parent = new BinaryOperatorNode("/");
        parent->set_left_operand(left);
        parent->set_right_operand(right);
        if (parent->get_right_operand()->evaluate() == 0.0) {
            std::cout << "Error: division by zero" << std::endl;
            return left;
        }
        return mult_div_list(parent);
    } else { 
        return left;
        // ε production, do nothing
    }
}

/*
<unary_expression> ::= + <unary_expression>
                     | - <unary_expression>
                     | <pow_expression>
*/
parser::ASTNode* parser::Parser::unary_expression() {
    if (current_token_type == "PLUS") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where PLUS should be parsed " << std::endl;
        current_token_type = scanner.get_next_token().get_type();
        UnaryOperatorNode* unary_node = new UnaryOperatorNode("+");
        unary_node->set_operand(unary_expression());
        return unary_node;
    } else if (current_token_type == "MINUS") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where MINUS should be parsed "<< std::endl;
        current_token_type = scanner.get_next_token().get_type();
        UnaryOperatorNode* unary_node = new UnaryOperatorNode("-");
        unary_node->set_operand(unary_expression());
        return unary_node;
    } else {
        return pow_expression();
    }
}

//<pow_expression> ::= <par/function_expression> <pow_list>
parser::ASTNode* parser::Parser::pow_expression() {
    ASTNode* left = par_function_expression();
    return pow_list(left);
}

/*
<pow_list> ::= ^ <unary_expression> <pow_list>
             | ε
*/
parser::ASTNode* parser::Parser::pow_list(ASTNode* left) {
    if (current_token_type == "POW_OP") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where POW_OP should be parsed " << std::endl;
        current_token_type = scanner.get_next_token().get_type();
        ASTNode* right = unary_expression();
        BinaryOperatorNode* parent = new BinaryOperatorNode("^");
        parent->set_left_operand(left);
        parent->set_right_operand(right);
        return pow_list(parent);
    } else {
        return left;
        // ε production, do nothing
    }
}

/*
<par/function_expression> ::=   <trigonometric_function> ( <math_expression> )
                            |   ( <math_expression> )
                            |   IDENTIFIER.val            // value has to be entered by the user
                            |   NUMBER         // it can be FLOAT or DOUBLE or INT
*/
parser::ASTNode* parser::Parser::par_function_expression() {
    if (current_token_type == "SIN" || current_token_type == "COS" ||
        current_token_type == "TAN" || current_token_type == "SEN") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where FUNC should be parsed " << std::endl;
        std::string function_parsed;
        if (current_token_type == "SIN" || current_token_type == "SEN") {
            function_parsed = "sin";
        }
        else if (current_token_type == "COS") {
            function_parsed = "cos";
        }
        else {
            function_parsed = "tan";
        }
        trigonometric_function();
        if (current_token_type == "LPAR") {
            std::cout << "TOKENS PARSED:" << current_token_type <<  " where LPAR should be parsed " << std::endl;
            current_token_type = scanner.get_next_token().get_type();
            if (current_token_type == "RPAR") {
                std::cout << "Error: Function '" << function_parsed
                        << "' expects 1 argument, but 0 were given." << std::endl;
                return nullptr;
            }
            FunctionNode* function_node = new FunctionNode(function_parsed);
            function_node->set_argument(math_expression()); 
            if (function_node->get_argument() == nullptr) {
                std::cout << "Error: invalid function argument." << std::endl;
                return nullptr;
            }
            if (current_token_type == "RPAR") {
                std::cout << "TOKENS PARSED:" << current_token_type <<  " where RPAR1 should be parsed "<< std::endl;
                current_token_type = scanner.get_next_token().get_type();
                return function_node;
            } else {
                std::cout << "Error: Expected ')'" << std::endl;
                return nullptr;
            }
        } else {
            std::cout << "Error: Expected '('" << std::endl;
            return nullptr;
        }
    } else if (current_token_type == "LPAR") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where LPAR should be parsed " << std::endl;
        current_token_type = scanner.get_next_token().get_type();
        ASTNode* expression = math_expression();
        if (current_token_type == "RPAR") {
            std::cout << "TOKENS PARSED:" <<  " where RPAR2 should be parsed "<< current_token_type << std::endl;
            current_token_type = scanner.get_next_token().get_type();
            return expression;
        } else {
            std::cout << "Error: Expected ')'" << std::endl;
            return nullptr;
        }
    } else if (current_token_type == "ID") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where ID should be parsed " << std::endl;
        
        // Variable detection
        std::string var_name = scanner.get_current_token().get_value();
        if (!env.exists(var_name)) {
            double storage_value;
            std::cout << "Variable '" << var_name << "' detected." << std::endl << "Please enter the corresponding value: ";
            std::cin >> storage_value;
            env.register_variable(var_name, storage_value);
        }
        IdentifierNode* identifier_node = new IdentifierNode(var_name, env.get_variable(var_name).value());
        current_token_type = scanner.get_next_token().get_type();
        return identifier_node;
    } else if (current_token_type == "INT_NUM" || current_token_type == "FLOAT_NUM" ||
               current_token_type == "DOUBLE_NUM") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where NUM should be parsed " << std::endl;
        NumberNode* number_node = new NumberNode(std::stod(scanner.get_current_token().get_value()));
        current_token_type = scanner.get_next_token().get_type();
        return number_node;
    } else if (current_token_type == "PI") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where PI should be parsed " << std::endl;
        NumberNode* number_node = new NumberNode(M_PI);
        current_token_type = scanner.get_next_token().get_type();
        return number_node;
    }
    else if (current_token_type == "E") {
        std::cout << "TOKENS PARSED:" << current_token_type <<  " where PI should be parsed " << std::endl;
        NumberNode* number_node = new NumberNode(M_E);
        current_token_type = scanner.get_next_token().get_type();
        return number_node;
    }
     
    else {
        std::cout << "Error: Unexpected token '" << current_token_type << "'" << std::endl;
        return nullptr;
    }
}

//<trigonometric_function> ::= sin | cos | tan | sen
void parser::Parser::trigonometric_function() {
    if (current_token_type == "SIN" || current_token_type == "COS" ||
        current_token_type == "TAN" || current_token_type == "SEN") {
        current_token_type = scanner.get_next_token().get_type();
    } else {
        std::cout << "Error: Expected a trigonometric function (sin, cos, tan, sen)" << std::endl;
    }
}

void parser::Parser::reset() {
    ast.set_root(nullptr);
    current_token_type = "NONE";
    env.clear();
}

VariableEnvironment parser::Parser::get_env() {
    return env;
}

void parser::Parser::set_env(VariableEnvironment env) {
    this->env = env;
}

void parser::Parser::set_scanner(lexer::Scanner& new_scanner) {
    scanner = new_scanner;
} 