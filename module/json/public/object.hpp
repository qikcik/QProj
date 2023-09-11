#pragma once

#include <vector>
#include <variant>
#include <string>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <memory>

namespace json
{
    using String = std::string;
    using Number = float;

    struct Object;
    struct Array;
    struct Value;

    using innerType = std::variant<Value,Object,Array>;

    struct Value
    {
        using innerType = std::variant<String,Number>;
        innerType value;

        template<typename T>
        T get() {return std::get<T>(value);};
    };

    struct Array
    {
        std::vector<innerType> values;

        template<typename T>
        T get(size_t idx);

    };

    struct Object
    {
        template<typename T>
        T get(const std::string& key);

        std::unordered_map<String,innerType> entries;

        std::string stringify() const;
    };

    template<typename T>
    T Array::get(size_t idx) {return std::get<T>(values[idx]);};

    template<typename T>
    T Object::get(const std::string& key) {return std::get<T>(entries[key]);};
}