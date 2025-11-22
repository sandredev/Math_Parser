#pragma once
#include "ast.hpp"
#include "../lexer/scanner.hpp"
#include "../variable_environment.hpp"

namespace parser {
    class Parser {
        private:
            /*
            For storing the scanner instance.
            It is used to get the tokens for parsing
            */
            lexer::Scanner scanner;

            /*
            Tree that contains the operations that will be solved.
            */
            AST ast;
            
            /*
            Strings that contains the name of the current token unit 
            being parsed.
            */
            std::string current_token_type;

            VariableEnvironment env;

            // Grammar
            ASTNode* math_expression();
            ASTNode* sum_substract_list(ASTNode* left);
            ASTNode* mult_div_list(ASTNode* left);
            ASTNode* mult_div_expression();
            ASTNode* unary_expression();
            ASTNode* pow_expression();
            ASTNode* pow_list(ASTNode* left);
            ASTNode* par_function_expression();
            void trigonometric_function();
        public:
            Parser(lexer::Scanner& scanner);

            AST get_ast() const;

            /*
            Evaluates the parsed expression and returns its numerical result.
            */
            double parse();

            void reset();

            VariableEnvironment get_env();
            void set_env(VariableEnvironment env);

            void set_scanner(lexer::Scanner& scanner);
    };
}