#pragma once
#include <string>
#include <unordered_map>
#include <memory>

struct Value {
    virtual ~Value() = default;
    virtual std::string to_string() const = 0;
};

struct IntValue : Value {
    int value;
    IntValue(int v) : value(v) {}
    std::string to_string() const override { return std::to_string(value); }
};

using ValuePtr = std::shared_ptr<Value>;
using RegisterFile = std::unordered_map<std::string, ValuePtr>;