#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "../frontend/tokens.hpp"
#include "../common/nodes.hpp"
#include "values.hpp"

class Interpreter {
public:
Interpreter();
    void interpret(const std::vector<std::unique_ptr<ASTNode>>& nodes);
    void dump_registers() const;

private:
    RegisterFile registers;
    std::unordered_map<std::string, size_t> label_table;
    Flags flags;

    void exec_node(const ASTNode* node, size_t& ip, const std::vector<std::unique_ptr<ASTNode>>& nodes);
    void exec_assignment(const AssignmentNode* node);
    void exec_binop(const BinOpNode* node);
    void exec_cmp(const CmpNode* node);

    int last_cmp_result = 0;
    int eval_node(const ASTNode* node) const;
    bool should_jump(const std::string& op) const;
};