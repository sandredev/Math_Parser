#pragma once
#include <string>

namespace parser
{
    // Abstract Syntax Tree Node base class
    class ASTNode
    {
        protected:
            std::string node_type;
        public:
            ASTNode(std::string node_type);

            virtual ~ASTNode() = default;

            std::string get_node_type() const;

            virtual std::string to_string() const = 0;

            virtual double evaluate() const = 0;
    };

    class NumberNode : public ASTNode
    {
        private:
            double value;

        public:
            NumberNode(double value);

            double evaluate() const override;

            std::string to_string() const override;
    };

    class IdentifierNode : public ASTNode
    {
        private:
            double storage_value; // To hold the value assigned to the identifier
            std::string name;
        public:
            IdentifierNode(std::string name, double storage_value);

            double evaluate() const override;

            std::string to_string() const override;
    };
    class BinaryOperatorNode : public ASTNode
    {
        private:
            std::string op;
            ASTNode* left_operand;
            ASTNode* right_operand;
        public:
            BinaryOperatorNode(std::string op);

            double evaluate() const override;

            void set_left_operand(ASTNode* left_operand);
            
            void set_right_operand(ASTNode* right_operand);

            std::string to_string() const override;

            ASTNode* get_left_operand();
            ASTNode* get_right_operand();

            virtual ~BinaryOperatorNode();
    };

    class UnaryOperatorNode : public ASTNode
    {
        private:
            std::string op;
            ASTNode* operand;
        public:
            UnaryOperatorNode(std::string op);

            virtual ~UnaryOperatorNode();

            double evaluate() const override;

            void set_operand(ASTNode* operand);

            std::string to_string() const override;
    };

    class FunctionNode : public ASTNode
    {
        private:
            ASTNode* argument;
            std::string function_name;
        public:
            FunctionNode(std::string function_name);

            void set_argument(ASTNode* argument);

            double evaluate() const override;

            std::string to_string() const override;

            ASTNode* get_argument();

            virtual ~FunctionNode();
    };
    class AST
    {
        private:
            ASTNode* root;

        public:
            AST(ASTNode* root);

            AST();

            ASTNode* get_root() const;

            virtual ~AST();

            void set_root(ASTNode* root);

            double evaluate() const;

            std::string to_string() const;
    };
}