#pragma once
#include "object.hpp"
#include "qstructType.hpp"
#include "qstructField.hpp"
#include "parser.hpp"

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

class Converter
{
public:
    template<QStruct TStruct>
    json::Object qstructToJson(TStruct& obj);

    template<QStruct TStruct>
    TStruct jsonToQStruct(const std::string& source);
};




json::Object qstructToJson_inner(void* obj,const QStructType& type)
{
    json::Object result {};
    for(const auto& fieldIt : type.getFields())
    {
        std::visit(overloaded {
                [&](const FieldType::Unimplemented& asUnimplemented)  {
                },
                [&](const FieldType::Float& asFloat)  {
                    auto value = fieldIt.getValueRef<float>(obj);
                    result.set(fieldIt.name,json::Value(value));
                },
                [&](const FieldType::StdString& asString)  {
                    auto value = fieldIt.getValueRef<std::string>(obj);
                    result.set(fieldIt.name,json::Value(value));
                },
                [&](const FieldType::QStruct& asQStruct)  {
                    auto inner_type = *asQStruct.type;
                    void* value = fieldIt.getValuePtr<void>(obj);
                    result.set(fieldIt.name,qstructToJson_inner(value,inner_type));
                }
        },fieldIt.type);
    }

    return result;
}

template<QStruct TStruct>
json::Object Converter::qstructToJson(TStruct& obj)
{
    QStructType type = obj.staticType;
    return qstructToJson_inner(&obj,type);
}


void jsonToQStruct_inner(json::Object& json,const QStructType& type, void* obj)
{
    for( auto& entryIt : json.entries )
    {
        auto field = type.getField(entryIt.first);
        std::visit(overloaded {
                [&](const FieldType::Unimplemented& asUnimplemented)  {
                },
                [&](const FieldType::Float& asFloat)  {
                    field.getValueRef<float>(obj) = std::get<json::Value>(entryIt.second).get<float>();
                },
                [&](const FieldType::StdString& asString)  {
                    field.getValueRef<std::string>(obj) = std::get<json::Value>(entryIt.second).get<std::string>();
                },
                [&](const FieldType::QStruct& asQStruct)  {
                    auto inner_json = std::get<json::Object>(entryIt.second);
                    void* value = field.getValuePtr<void>(obj);
                    jsonToQStruct_inner(inner_json,*asQStruct.type,value);
                }
        },field.type);
    }
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
        jsonToQStruct_inner(*json,type,&result);
    }

    return result;
}