#pragma once
#include "object.hpp"
#include "qstructType.hpp"
#include "qstructField.hpp"
#include "parser.hpp"
#include "abandon/fixedArrayScriptHelper.hpp"
#include "dynamicArrayScriptHelper.hpp"

namespace Converter
{
    template<QStruct TStruct>
    json::Object qstructToJson(TStruct& obj);
    json::Object qstructToJson(void* obj,const QStructType& type);

    template<QStruct TStruct>
    TStruct jsonToQStruct(const std::string& source);
    void jsonToQStruct_inner(json::Object& json,const QStructType& type, void* obj);
};


template<QStruct TStruct>
json::Object Converter::qstructToJson(TStruct& obj)
{
    QStructType type = obj.staticType;
    return qstructToJson(&obj,type);
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
