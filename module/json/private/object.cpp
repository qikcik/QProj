#include "object.hpp"
using namespace json;

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


std::string stringifyArr( const innerType val)
{
    std::string result;
    std::visit(overloaded{
            [&](const Value& str)
            {
                result = "\""+str.value+"\",";
            },
            [&](const Object& obj)
            {
                result = obj.stringify()+",";
            },
            [&](const Array& arr)
            {
                result = "[";

                for(const auto& it : arr.values)
                    result += stringifyArr(it);

                result[result.size()-1] = ']';
                result += ",";
            },
    },val);

    return "";
}

std::string Object::stringify() const
{
    std::string result = "{";

    for(const auto& entryIt : entries)
    {
        result += "\""+entryIt.first+"\":";

        std::visit(overloaded {
            [&](const Value& str)
            {
                result += "\""+str.value+"\",";
            },
            [&](const Object& obj)
            {
                result += obj.stringify()+",";
            },
            [&](const Array& arr)
            {
                result += "[";
                for(const auto& strIt : arr.values)
                    std::visit(overloaded{
                        [&](const Value& str)
                        {
                            result += "\""+str.value+"\",";
                        },
                        [&](const Object& obj)
                        {
                            result += obj.stringify()+",";
                        },
                        [&](const Array& arr)
                        {
                            result += "[";
                            for(const auto& vecIt : arr.values)
                                result += stringifyArr(vecIt);
                            result[result.size()-1] = ']';
                            result += ",";
                        },
                    },strIt);

                result[result.size()-1] = ']';
                result += ",";
            },
        }, entryIt.second);
    }

    result[result.size()-1] = '}';
    return result;
}