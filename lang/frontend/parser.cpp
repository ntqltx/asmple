#include "parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token>& toks)
    : tokens(toks), token_idx(-1), current_token(Token(TokenType::EOF_TOKEN, "")) {
    advance();
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::IDENT: return "IDENT";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::EQ: return "EQ";
        case TokenType::COMMA: return "COMMA";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::LABEL: return "LABEL";
        case TokenType::COLON: return "COLON";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        default: return "UNKNOWN";
    }
}

void Parser::advance() {
    token_idx++;
    if (token_idx < tokens.size())
        current_token = tokens[token_idx];
    else
        current_token = Token(TokenType::EOF_TOKEN, "");
}

bool Parser::is_at_end() const {
    return current_token.type == TokenType::EOF_TOKEN;
}

void Parser::skip_newlines() {
    while (current_token.type == TokenType::NEWLINE)
        advance();
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> nodes;
    while (!is_at_end()) {
        skip_newlines();
        if (is_at_end()) break;
        auto node = statement();
        if (node) nodes.push_back(std::move(node));
        skip_newlines();
    }
    return nodes;
}

std::unique_ptr<ASTNode> Parser::statement() {
    if (current_token.type == TokenType::KEYWORD) {
        if (current_token.value == "let") return assignment();
        if (current_token.value == "add" || current_token.value == "sub" ||
            current_token.value == "mul" || current_token.value == "div")
            return parse_binop();
        if (current_token.value == "jmp" || current_token.value == "je" ||
            current_token.value == "jne" || current_token.value == "jl" ||
            current_token.value == "jg" || current_token.value == "jle" ||
            current_token.value == "jge")
            return parse_jump();
        if (current_token.value == "cmp") return parse_cmp();
        if (current_token.value == "print") return parse_print();
    }
    if (current_token.type == TokenType::LABEL)
        return parse_label();
    return expression();
}

std::unique_ptr<ASTNode> Parser::assignment() {
    advance();
    if (current_token.type != TokenType::IDENT) return nullptr;
    Token var_token = current_token;
    advance();
    if (current_token.type != TokenType::EQ) return nullptr;
    advance();
    auto value = expression();
    return std::make_unique<AssignmentNode>(var_token, std::move(value));
}

std::unique_ptr<ASTNode> Parser::expression() {
    if (current_token.type == TokenType::INT)
        return parse_number();
    if (current_token.type == TokenType::IDENT)
        return parse_identifier();
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parse_number() {
    auto node = std::make_unique<NumberNode>(current_token);
    advance();
    return node;
}

std::unique_ptr<ASTNode> Parser::parse_identifier() {
    auto node = std::make_unique<IdentifierNode>(current_token);
    advance();
    return node;
}

std::unique_ptr<ASTNode> Parser::parse_binop() {
    Token op_token = current_token; // "add", "sub", etc.
    advance();
    if (current_token.type != TokenType::IDENT) return nullptr;
    auto left = parse_identifier();
    if (current_token.type != TokenType::COMMA) return nullptr;
    advance();
    auto right = expression();
    return std::make_unique<BinOpNode>(std::move(left), op_token, std::move(right));
}

std::unique_ptr<ASTNode> Parser::parse_label() {
    std::string label_name = current_token.value;
    advance();
    return std::make_unique<LabelNode>(label_name);
}

std::unique_ptr<ASTNode> Parser::parse_jump() {
    std::string jump_type = current_token.value;
    advance();
    if (current_token.type != TokenType::IDENT) return nullptr;
    std::string label_name = current_token.value;
    advance();
    return std::make_unique<JumpNode>(jump_type, label_name);
}

std::unique_ptr<ASTNode> Parser::parse_cmp() {
    advance();
    auto left = expression();
    if (!left) {
        std::cerr << "Parser error: Expected expression after 'cmp', got " << current_token.value << std::endl;
        advance();
        return nullptr;
    }

    if (current_token.type != TokenType::COMMA) {
        std::cerr << "Parser error: Expected ',' after first expression in cmp, got " << current_token.value << std::endl;
        advance();
        return nullptr;
    }
    advance();
    auto right = expression();
    if (!right) {
        std::cerr << "Parser error: Expected right-hand expression in cmp, got " << current_token.value << std::endl;
        advance();
        return nullptr;
    }
    return std::make_unique<CmpNode>(std::move(left), std::move(right));
}

std::unique_ptr<ASTNode> Parser::parse_print() {
    advance();
    auto expr = expression();
    if (!expr) {
        std::cerr << "Parser error: Expected expression after 'print'\n";
        return nullptr;
    }
    return std::make_unique<PrintNode>(std::move(expr));
}