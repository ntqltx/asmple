#include <fstream>
#include <iostream>
#include <sstream>
#include "../frontend/lexer.hpp"
#include "../frontend/parser.hpp"
#include "../backend/interpreter.hpp"

int main(int argc, char* argv[]) {
    std::string filename = "tests/lang/example.asmp";
    if (argc > 1) filename = argv[1];

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open " << filename << "\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    Interpreter interpreter;
    interpreter.interpret(ast);

    // interpreter.dump_registers();
    return 0;
}