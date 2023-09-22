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
    using Float = float;

    struct Object;
    struct Array;
    struct Value;

    using innerType = std::variant<Value,Object,Array>;

    struct Value
    {
        Value() = default;

        explicit Value(const std::string& val) : value(val) {};
        explicit Value(float val) : value(val) {};
        using innerType = std::variant<String,Float>;
        innerType value {};

        template<typename T>
        T get() const {return std::get<T>(value);};
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

        void set(const std::string& key,const innerType& value);

        std::unordered_map<String,innerType> entries;

        std::string stringify() const;
    };

    template<typename T>
    T Array::get(size_t idx) {return std::get<T>(values[idx]);};

    template<typename T>
    T Object::get(const std::string& key) {return std::get<T>(entries[key]);};
}