#pragma once
#include <vector>
#include <memory>
#include "../frontend/tokens.hpp"
#include "../common/nodes.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    std::vector<std::unique_ptr<ASTNode>> parse();

private:
    std::vector<Token> tokens;
    int token_idx;
    Token current_token;

    void advance();
    bool is_at_end() const;
    void skip_newlines();

    std::unique_ptr<ASTNode> statement();
    std::unique_ptr<ASTNode> assignment();
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> parse_number();
    std::unique_ptr<ASTNode> parse_identifier();
    std::unique_ptr<ASTNode> parse_binop();
    std::unique_ptr<ASTNode> parse_label();
    std::unique_ptr<ASTNode> parse_jump();
    std::unique_ptr<ASTNode> parse_cmp();
    std::unique_ptr<ASTNode> parse_print();
};