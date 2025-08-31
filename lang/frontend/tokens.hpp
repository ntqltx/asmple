#pragma once
#include <string>
#include <vector>

struct Flags {
    bool equal = false;
    bool less = false;
    bool greater = false;
};

enum class TokenType {
    INT,
    IDENT,
    KEYWORD,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQ,
    COMMA,
    NEWLINE,
    LABEL,
    COLON,
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, std::string v, int l = 0, int c = 0)
        : type(t), value(std::move(v)), line(l), column(c) {}
};

inline const std::vector<std::string> keywords = {
    "let", "add", "sub", "mul", "div",
    "cmp", "jmp", "je", "jne", "jl",
    "jg", "jle", "jge", "print"
};