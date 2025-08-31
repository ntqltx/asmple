#include "interpreter.hpp"
#include <iostream>
#include <stdexcept>

Interpreter::Interpreter() {}

void Interpreter::interpret(const std::vector<std::unique_ptr<ASTNode>>& nodes) {
    label_table.clear();
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i]->type_name() == "LabelNode") {
            auto* label = static_cast<const LabelNode*>(nodes[i].get());
            label_table[label->label] = i;
        }
    }

    for (size_t ip = 0; ip < nodes.size(); ) {
        const ASTNode* node = nodes[ip].get();
        if (node->type_name() == "JumpNode") {
             auto* jump = static_cast<const JumpNode*>(node);
            if (should_jump(jump->op) && label_table.count(jump->label)) {
                ip = label_table[jump->label] + 1;
            } else {
                ++ip;
            }
        } else {
            exec_node(node, ip, nodes);
            ++ip;
        }
    }
}

void Interpreter::exec_node(const ASTNode* node, size_t& ip, const std::vector<std::unique_ptr<ASTNode>>& nodes) {
    if (!node) return;

    if (node->type_name() == "PrintNode") {
        auto* print = static_cast<const PrintNode*>(node);
        int value = eval_node(print->expr.get());
        std::cout << value << std::endl;
    }
    if (node->type_name() == "AssignmentNode")
        exec_assignment(static_cast<const AssignmentNode*>(node));
    else if (node->type_name() == "BinOpNode")
        exec_binop(static_cast<const BinOpNode*>(node));
    else if (node->type_name() == "CmpNode")
        exec_cmp(static_cast<const CmpNode*>(node));
}

int Interpreter::eval_node(const ASTNode* node) const {
    if (!node) throw std::runtime_error("Null AST node in eval_node");

    if (node->type_name() == "NumberNode") {
        return std::stoi(static_cast<const NumberNode*>(node)->token.value);
    }
    if (node->type_name() == "IdentifierNode") {
        std::string reg = static_cast<const IdentifierNode*>(node)->token.value;
        if (registers.count(reg))
            return static_cast<IntValue*>(registers.at(reg).get())->value;
        throw std::runtime_error("Unknown register: " + reg);
    }
    if (node->type_name() == "BinOpNode") {
        auto* binop = static_cast<const BinOpNode*>(node);
        int lhs = eval_node(binop->left.get());
        int rhs = eval_node(binop->right.get());
        std::string op = binop->op_token.value;
        if (op == "add") return lhs + rhs;
        else if (op == "sub") return lhs - rhs;
        else if (op == "mul") return lhs * rhs;
        else if (op == "div") return (rhs != 0) ? (lhs / rhs) : 0;
        else throw std::runtime_error("Unknown binop: " + op);
    }
    throw std::runtime_error("Unsupported AST node in eval_node: " + node->type_name());
}

void Interpreter::exec_assignment(const AssignmentNode* node) {
    int val = eval_node(node->value.get());
    registers[node->var_token.value] = std::make_shared<IntValue>(val);
}

void Interpreter::exec_binop(const BinOpNode* node) {
    std::string reg = static_cast<const IdentifierNode*>(node->left.get())->token.value;
    int lhs = 0;
    if (registers.count(reg))
        lhs = static_cast<IntValue*>(registers[reg].get())->value;
    int rhs = eval_node(node->right.get());

    int result = lhs;
    std::string op = node->op_token.value;
    if (op == "add") result = lhs + rhs;
    else if (op == "sub") result = lhs - rhs;
    else if (op == "mul") result = lhs * rhs;
    else if (op == "div") result = (rhs != 0) ? (lhs / rhs) : 0;

    registers[reg] = std::make_shared<IntValue>(result);
}

void Interpreter::exec_cmp(const CmpNode* node) {
    int lhs = eval_node(node->left.get());
    int rhs = eval_node(node->right.get());
    flags.equal = (lhs == rhs);
    flags.less = (lhs < rhs);
    flags.greater = (lhs > rhs);

    // std::cout << "cmp: " << lhs << " vs " << rhs
    //           << " | eq=" << flags.equal
    //           << " lt=" << flags.less
    //           << " gt=" << flags.greater << std::endl;
}

void Interpreter::dump_registers() const {
    std::cout << "Registers:\n";
    for (const auto& kv : registers) {
        std::cout << "  " << kv.first << " = " << kv.second->to_string() << "\n";
    }
}

bool Interpreter::should_jump(const std::string& op) const {
    if (op == "jmp") return true;
    if (op == "je")  return flags.equal;
    if (op == "jne") return !flags.equal;
    if (op == "jl")  return flags.less;
    if (op == "jle") return flags.less || flags.equal;
    if (op == "jg")  return flags.greater;
    if (op == "jge") return flags.greater || flags.equal;
    return false;
}