#include "lexer.hpp"
#include <cctype>

Lexer::Lexer(const std::string& src)
    : source(src), pos(0), line(1), column(0), current_char('\0') {
    if (!source.empty())
        current_char = source[0];
}

void Lexer::advance() {
    if (current_char == '\n') {
        ++line;
        column = 0;
    } else {
        ++column;
    }
    if (++pos < source.size())
        current_char = source[pos];
    else
        current_char = '\0';
}

char Lexer::peek() const {
    if (pos + 1 < source.size())
        return source[pos + 1];
    return '\0';
}

void Lexer::skip_whitespace() {
    while (current_char == ' ' || current_char == '\t' || current_char == '\r')
        advance();
}

void Lexer::skip_comment() {
    while (current_char && current_char != '\n')
        advance();
}

bool Lexer::is_at_end() const {
    return current_char == '\0';
}

bool Lexer::is_digit(char c) const {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Lexer::is_alpha(char c) const {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool Lexer::is_alnum(char c) const {
    return std::isalnum(static_cast<unsigned char>(c));
}

Token Lexer::make_number() {
    std::string value;
    int start_col = column;
    while (is_digit(current_char)) {
        value += current_char;
        advance();
    }
    return Token(TokenType::INT, value, line, start_col);
}

Token Lexer::make_identifier_or_keyword() {
    std::string value;
    int start_col = column;
    while (is_alnum(current_char) || current_char == '_') {
        value += current_char;
        advance();
    }
    TokenType type = TokenType::IDENT;
    for (const auto& kw : keywords) {
        if (value == kw) {
            type = TokenType::KEYWORD;
            break;
        }
    }
    return Token(type, value, line, start_col);
}

Token Lexer::make_label() {
    std::string value;
    int start_col = column;
    while (is_alnum(current_char) || current_char == '_') {
        value += current_char;
        advance();
    }
    if (current_char == ':') {
        advance();
        return Token(TokenType::LABEL, value, line, start_col);
    }
    return Token(TokenType::IDENT, value, line, start_col);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!is_at_end()) {
        skip_whitespace();

        if (current_char == ';') {
            skip_comment();
            continue;
        }

        if (is_digit(current_char)) {
            tokens.push_back(make_number());
            continue;
        }

        if (is_alpha(current_char) && peek() != '\0') {
            size_t temp_pos = pos;
            while ((is_alnum(source[temp_pos]) || source[temp_pos] == '_') && temp_pos < source.size())
                ++temp_pos;
            if (source[temp_pos] == ':') {
                tokens.push_back(make_label());
                continue;
            }
        }

        if (is_alpha(current_char) || current_char == '_') {
            tokens.push_back(make_identifier_or_keyword());
            continue;
        }

        if (current_char == '+') {
            tokens.emplace_back(TokenType::PLUS, "+", line, column);
            advance();
            continue;
        }
        if (current_char == '-') {
            tokens.emplace_back(TokenType::MINUS, "-", line, column);
            advance();
            continue;
        }
        if (current_char == '*') {
            tokens.emplace_back(TokenType::MUL, "*", line, column);
            advance();
            continue;
        }
        if (current_char == '/') {
            tokens.emplace_back(TokenType::DIV, "/", line, column);
            advance();
            continue;
        }
        if (current_char == '=') {
            tokens.emplace_back(TokenType::EQ, "=", line, column);
            advance();
            continue;
        }
        if (current_char == ',') {
            tokens.emplace_back(TokenType::COMMA, ",", line, column);
            advance();
            continue;
        }
        if (current_char == ':') {
            tokens.emplace_back(TokenType::COLON, ":", line, column);
            advance();
            continue;
        }
        if (current_char == '\n') {
            tokens.emplace_back(TokenType::NEWLINE, "\\n", line, column);
            advance();
            continue;
        }

        advance();
    }
    tokens.emplace_back(TokenType::EOF_TOKEN, "", line, column);
    return tokens;
}