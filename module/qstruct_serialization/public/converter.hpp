#pragma once
#include "object.hpp"
#include "qstructType.hpp"
#include "qstructField.hpp"
#include "parser.hpp"
#include "fixedArrayScriptHelper.hpp"

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


inline json::Object qstructToJson_inner(void* obj,const QStructType& type)
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
                },
                [&](const FieldType::FixedArray& asFixedArray)  {
                    void* value = fieldIt.getValuePtr<void>(obj);
                    qstd::FixedArrayScriptHelper helper { value,sizeof(float) };

                    result.set(fieldIt.name,json::Array());
                    auto& arr = result.get<json::Array>(fieldIt.name);
                    for(size_t idx=0; idx != helper.get_length(); idx++)
                    {
                        void* ptr = helper.get_elementPtr(idx);
                        arr.values.push_back( json::Value(*reinterpret_cast<float*>(ptr)));
                    }
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


inline void jsonToQStruct_inner(json::Object& json,const QStructType& type, void* obj)
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
                    auto& inner_json = std::get<json::Object>(entryIt.second);
                    void* value = field.getValuePtr<void>(obj);
                    jsonToQStruct_inner(inner_json,*asQStruct.type,value);
                },
                [&](const FieldType::FixedArray& asFixedArray)  {
                    void* value = field.getValuePtr<void>(obj);
                    qstd::FixedArrayScriptHelper helper { value,sizeof(float) };

                    auto& inner_json = std::get<json::Array>(entryIt.second);
                    helper.set_length(inner_json.values.size());
                    int i=0;
                    for( auto& valueIt : inner_json.values)
                    {
                        auto asFloat = std::get<json::Value>(valueIt).get<float>();
                        auto* elPtr = helper.get_elementPtr(i);
                        *reinterpret_cast<float*>(elPtr) = asFloat;
                        i++;
                    }
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