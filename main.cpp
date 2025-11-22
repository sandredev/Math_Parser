#include "include/lexer/scanner.hpp"
#include "include/parser/parser.hpp"
#include <string>
#include <iostream>
#include "include/variable_environment.hpp"

int init_program() {
    std::cout << "Initializing Math Parser..." << std::endl;
    int option = 0;
    std::string input_expression;
    std::string default_expression;
    std::string token; 
    while (option != 3) {
        std::cout << "Please select an option:" << std::endl;
        std::cout << "1. Input expression manually" << std::endl;
        std::cout << "2. Test cases" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Option: ";
        std::cin >> option;
        switch (option)
        {
        case 1: 
            std::cout << "Please input the mathematical expression to parse:" << std::endl;
            std::cin.ignore();// To ignore the newline character left in the buffer
            std::getline(std::cin, input_expression);
            std::cout << "You entered: " << input_expression << std::endl;
            std::cout << "Tokenizing the expression..." << std::endl;
            {
                lexer::Scanner scanner(input_expression);
                scanner.scan(); // Only for debugging, not really useful
                parser::Parser parser(scanner);
                double result = parser.parse();
                std::cout << std::endl << "Your result is: " << result << std::endl;
            } // To limit the scope of 'scanner' variable
            break;
        case 2:
            {   
                double result1, result2, result3, result4, result5,
                    result6, result7, result8, result9, result10, result18, result19, result20;
                lexer::Scanner scanner1("3 + 4 * 2");
                parser::Parser parser1(scanner1);
                result1 = parser1.parse();
                lexer::Scanner scanner2("(3 + 4) * 2");
                parser::Parser parser2(scanner2);
                result2 = parser2.parse();
                lexer::Scanner scanner3("-2^2");
                parser::Parser parser3(scanner3);
                result3 = parser3.parse();
                lexer::Scanner scanner4("2 ^ 3 ^ 2");
                parser::Parser parser4(scanner4);;
                result4 = parser4.parse();
                lexer::Scanner scanner5("sin(pi / 2)");
                parser::Parser parser5(scanner5);;
                result5 = parser5.parse();
                lexer::Scanner scanner6("cos(0) + sin(pi)");
                parser::Parser parser6(scanner6);
                result6 = parser6.parse();
                lexer::Scanner scanner7("x * 2 + y");
                parser::Parser parser7(scanner7);
                VariableEnvironment env7;
                env7.register_variable("x", 3);
                env7.register_variable("y", 4);
                parser7.set_env(env7);
                result7 = parser7.parse();
                lexer::Scanner scanner8("3.5 * 2.0 + .5");
                parser::Parser parser8(scanner8);
                result8 = parser8.parse();
                lexer::Scanner scanner9("tan(pi/4)");
                parser::Parser parser9(scanner9);
                result9 = parser9.parse();
                lexer::Scanner scanner10("2 ^ (1/2)");
                parser::Parser parser10(scanner10);
                result10 = parser10.parse();
                lexer::Scanner scanner18("sin(0) + cos(0)");
                parser::Parser parser18(scanner18);
                result18 = parser18.parse();
                lexer::Scanner scanner19("(2 + 3) * (4 + 1)");
                parser::Parser parser19(scanner19);
                result19 = parser19.parse();
                lexer::Scanner scanner20("cos(pi) + sin(pi/2)");
                parser::Parser parser20(scanner20);
                result20 = parser20.parse();
                std::cout << std::endl << "Test case 1: 3 + 4 * 2";
                std::cout << std::endl << "Result 1: " << result1;
                std::cout << std::endl << "Test case 2: (3 + 4) * 2";
                std::cout << std::endl << "Result 2: " << result2;
                std::cout << std::endl << "Test case 3: -2^2";
                std::cout << std::endl << "Result 3: " << result3;
                std::cout << std::endl << "Test case 4: 2 ^ 3 ^ 2";
                std::cout << std::endl << "Result 4: " << result4;
                std::cout << std::endl << "Test case 5: sin(pi / 2)";
                std::cout << std::endl << "Result 5: " << result5;
                std::cout << std::endl << "Test case 6: cos(0) + sin(pi)";
                std::cout << std::endl << "Result 6: " << result6;
                std::cout << std::endl << "Test case 7: x * 2 + y; env = {x:3, y:4}";
                std::cout << std::endl << "Result 7: " << result7;
                std::cout << std::endl << "Test case 8: 3.5 * 2.0 + .5";
                std::cout << std::endl << "Result 8: " << result8;
                std::cout << std::endl << "Test case 9: tan(pi/4)";
                std::cout << std::endl << "Result 9: " << result9;
                std::cout << std::endl << "Test case 10: 2 ^ (1/2)";
                std::cout << std::endl << "Result 10: " << result10 << std::endl;
                //lexer::Scanner scanner11("3 + * 4");
                //parser::Parser parser11(scanner11);
                //std::cout << std::endl << "Test case 11: 3 + * 4";
                //std::cout << std::endl << "Errors: " << parser11.parse();
                //lexer::Scanner scanner12("sin()");
                //parser::Parser parser12(scanner12);
                //std::cout << std::endl << "Test case 12: sin()";
                //std::cout << std::endl << "Result 12: " << parser12.parse();
                lexer::Scanner scanner13("a + 3");
                parser::Parser parser13(scanner13);
                VariableEnvironment env13;  
                parser13.set_env(env13); // 'a' not stored
                double result13 = parser13.parse();
                std::cout << std::endl << "Test case 13: a + 3";
                std::cout << std::endl << "Result 13: " << result13; // no errors because this program doesn't let that happen
                lexer::Scanner scanner14("1 / 0");
                parser::Parser parser14(scanner14);
                std::cout << std::endl << "Test case 14: 1 / 0";
                std::cout << std::endl << "Processing: " << parser14.parse(); // find errors but controls it
                //lexer::Scanner scanner15("2 ^");
                //parser::Parser parser15(scanner15);
                //std::cout << std::endl << "Test case 15: 2 ^";
                //std::cout << std::endl << "Proccessing: " << parser15.parse();
                //lexer::Scanner scanner16("sin(1,2)");
                //parser::Parser parser16(scanner16);
                //std::cout << std::endl << "Test case 16: sin(1,2)";
                //std::cout << std::endl << "Processing: " << parser16.parse();
                lexer::Scanner scanner17("5..3");
                parser::Parser parser17(scanner17);
                std::cout << std::endl << "Test case 17: 5..3";
                std::cout << std::endl << "Processing: " << parser17.parse();
                std::cout << std::endl << "Test case 18: sin(0) + cos(0)";
                std::cout << std::endl << "Result 18: " << result18;
                std::cout << std::endl << "Test case 19: (2 + 3) * (4 + 1)";
                std::cout << std::endl << "Result 19: " << result19;
                std::cout << std::endl << "Test case 20: cos(pi) + sin(pi/2)";
                std::cout << std::endl << "Result 20: " << result20 << std::endl;
            }
            break;
        case 3:
            std::cout << "Exiting program..." << std::endl;
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
        }
    }
    
    return 0;
}
int main() {
    init_program();
    return 0;    
}