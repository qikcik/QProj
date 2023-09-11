#pragma once

#include <vector>
#include <variant>
#include <string>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <memory>
#include "object.hpp"

namespace json
{

    class Parser
    {
    public:
        struct ParseError {std::string description{}; int atChar{}; };
        std::variant<Object,ParseError> parse(const std::string& source);
    };
}