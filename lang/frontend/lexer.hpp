#pragma once
#include <string>
#include <vector>
#include "tokens.hpp"

class Lexer {
public:
    Lexer(const std::string& src);

    std::vector<Token> tokenize();

private:
    std::string source;
    size_t pos;
    int line;
    int column;
    char current_char;

    void advance();
    char peek() const;
    void skip_whitespace();
    void skip_comment();
    Token make_number();
    Token make_identifier_or_keyword();
    Token make_label();

    bool is_at_end() const;
    bool is_digit(char c) const;
    bool is_alpha(char c) const;
    bool is_alnum(char c) const;
};