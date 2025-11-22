#include "../../include/parser/ast.hpp"
#include <string>
#include <cmath>
#include <iostream>

std::string parser::ASTNode::get_node_type() const {
    return node_type;
}

parser::ASTNode::ASTNode(std::string node_type) : node_type(node_type) {};

parser::NumberNode::NumberNode(double value) : ASTNode("NUMBER"), value(value) {};

double parser::NumberNode::evaluate() const {
    return value;
}

std::string parser::NumberNode::to_string() const {
    return std::to_string(value);
}

parser::IdentifierNode::IdentifierNode(std::string name, double storage_value) : ASTNode("IDENTIFIER"), name(name), storage_value(storage_value) {}

std::string parser::IdentifierNode::to_string() const {
    return name;
}

double parser::IdentifierNode::evaluate() const {
    return storage_value;
}

parser::BinaryOperatorNode::BinaryOperatorNode(std::string op) : ASTNode("BINARY_OPERATOR"), op(op), left_operand(nullptr), right_operand(nullptr) {}

double parser::BinaryOperatorNode::evaluate() const {
    double left = left_operand->evaluate();
    double right = right_operand->evaluate();
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") return left / right;
    if (op == "^") return pow(left, right);
    return 0.0;
}

std::string parser::BinaryOperatorNode::to_string() const {
    return left_operand->to_string() + op + right_operand->to_string();
}


parser::BinaryOperatorNode::~BinaryOperatorNode() {
    delete left_operand;
    delete right_operand;
}


void parser::BinaryOperatorNode::set_left_operand(ASTNode* left_operand) {
    this->left_operand = left_operand;
}
void parser::BinaryOperatorNode::set_right_operand(ASTNode* right_operand) {
    this->right_operand = right_operand;
}

parser::ASTNode* parser::BinaryOperatorNode::get_left_operand() {
    return left_operand;
}
        
parser::ASTNode* parser::BinaryOperatorNode::get_right_operand() {
    return right_operand;
}

parser::UnaryOperatorNode::UnaryOperatorNode(std::string op) : ASTNode("UNARY_OPERATOR"), op(op), operand(nullptr) {}

double parser::UnaryOperatorNode::evaluate() const {
    if (op == "+") return operand->evaluate();
    else if (op == "-") return -operand->evaluate();
    else return 0.0;
}

std::string parser::UnaryOperatorNode::to_string() const {
    return op + operand->to_string();
}

void parser::UnaryOperatorNode::set_operand(ASTNode* operand) {
    this->operand = operand;
}

parser::UnaryOperatorNode::~UnaryOperatorNode() {
    delete operand;
}


parser::FunctionNode::FunctionNode(std::string function_name) : ASTNode("FUNCTION"), function_name(function_name), argument(nullptr) {}

double parser::FunctionNode::evaluate() const {
    double arg_num = argument->evaluate();
    if (function_name == "sin") return std::sin(arg_num);
    if (function_name == "cos") return std::cos(arg_num);
    if (function_name == "tan") return std::tan(arg_num);
    return 0.0;
}

void parser::FunctionNode::set_argument(ASTNode* argument) {
    this->argument = argument;
}

parser::ASTNode* parser::FunctionNode::get_argument() {
    return argument;
}

std::string parser::FunctionNode::to_string() const {
    return function_name + "(" + argument->to_string() + ")";
}

parser::FunctionNode::~FunctionNode() {
    delete argument;
}

parser::AST::AST(ASTNode* root) : root(root) {};

parser::AST::AST() : root(nullptr) {}

parser::ASTNode* parser::AST::get_root() const {
    return root;
}

void parser::AST::set_root(ASTNode* root) {
    this->root = root;
}

double parser::AST::evaluate() const {
    return root->evaluate();
}

parser::AST::~AST() {
    delete root;
}

std::string parser::AST::to_string() const {
    return root->to_string();
}