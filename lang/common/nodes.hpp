#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../frontend/tokens.hpp"

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual std::string type_name() const = 0;
};

struct NumberNode : ASTNode {
    Token token;
    NumberNode(const Token& t) : token(t) {}
    std::string type_name() const override { return "NumberNode"; }
};

struct IdentifierNode : ASTNode {
    Token token;
    IdentifierNode(const Token& t) : token(t) {}
    std::string type_name() const override { return "IdentifierNode"; }
};

struct AssignmentNode : ASTNode {
    Token var_token;
    std::unique_ptr<ASTNode> value;
    AssignmentNode(Token t, std::unique_ptr<ASTNode> v)
        : var_token(t), value(std::move(v)) {}
    std::string type_name() const override { return "AssignmentNode"; }
};

struct BinOpNode : ASTNode {
    std::unique_ptr<ASTNode> left;
    Token op_token;
    std::unique_ptr<ASTNode> right;
    BinOpNode(std::unique_ptr<ASTNode> l, Token op, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op_token(op), right(std::move(r)) {}
    std::string type_name() const override { return "BinOpNode"; }
};

struct PrintNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    PrintNode(std::unique_ptr<ASTNode> expr)
        : expr(std::move(expr)) {}
    std::string type_name() const override { return "PrintNode"; }
};

struct LabelNode : ASTNode {
    std::string label;
    LabelNode(const std::string& name) : label(name) {}
    std::string type_name() const override { return "LabelNode"; }
};

struct JumpNode : ASTNode {
    std::string op;
    std::string label;
    JumpNode(const std::string& jump_type, const std::string& label)
        : op(jump_type), label(label) {}
    std::string type_name() const override { return "JumpNode"; }
};

struct CmpNode : ASTNode {
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    CmpNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), right(std::move(r)) {}
    std::string type_name() const override { return "CmpNode"; }
};