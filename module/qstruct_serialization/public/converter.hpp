#pragma once
#include "object.hpp"
#include "qstructType.hpp"
#include "qstructField.hpp"
#include "parser.hpp"

class Converter
{
public:
    template<QStruct TStruct>
    json::Object qstructToJson(TStruct& obj);

    template<QStruct TStruct>
    TStruct jsonToQStruct(const std::string& source);
};


template<QStruct TStruct>
json::Object Converter::qstructToJson(TStruct& obj)
{
    QStructType type = obj.staticType;
    json::Object result {};

    for(const auto& fieldIt : type.getFields())
    {
        if (fieldIt.type == FieldType::Float)
        {
            float value = fieldIt.getValueRef<float>(&obj);
            result.set(fieldIt.name,json::Value(value));
        }
        if (fieldIt.type == FieldType::StdString)
        {
            std::string value = fieldIt.getValueRef<std::string>(&obj);
            result.set(fieldIt.name,json::Value(value));
        }
    }

    return result;
}

template<QStruct TStruct>
TStruct Converter::jsonToQStruct(const std::string& source)
{
    QStructType type = TStruct::staticType;
    TStruct result {};

    json::Parser parser {};
    auto ParserRes = parser.parse(source);
    if (auto* json = std::get_if<json::Object>(&ParserRes); json )
    {
        for( auto& entryIt : json->entries )
        {
            auto field = type.getField(entryIt.first);
            if(field.type == FieldType::Float)
            {
                field.getValueRef<float>(&result) = std::get<json::Value>(entryIt.second).get<float>();
            }
            if(field.type == FieldType::StdString)
            {
                field.getValueRef<std::string>(&result) = std::get<json::Value>(entryIt.second).get<std::string>();
            }
        }
    }


    return result;
}