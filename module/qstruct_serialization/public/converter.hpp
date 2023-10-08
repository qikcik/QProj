#pragma once
#include "object.hpp"
#include "qstructType.hpp"
#include "qstructField.hpp"
#include "parser.hpp"
#include "abandon/fixedArrayScriptHelper.hpp"
#include "dynamicArrayScriptHelper.hpp"

class Converter
{
public:
    Converter(const QDynamicInfo& inDynamicInfo = {}) : dynamicInfo(inDynamicInfo) {};

    template<QStruct TStruct>
    json::Object qstructToJson(TStruct& obj);
    json::Object qstructToJson(void* obj,const QStructType& type);

    template<QStruct TStruct>
    void jsonToQStruct(void* obj,const std::string& source);
    void jsonToQStruct_inner(json::Object& json,const QStructType& type, void* obj);


protected:

    json::innerType FieldToJson(void* addr,const FieldType::type& type);
    void jsonArrayToQstruct(json::Array& arrayJson,const FieldType::DynamicArray& dynamicArray, void* addr);

    const QDynamicInfo& dynamicInfo;
};


template<QStruct TStruct>
json::Object Converter::qstructToJson(TStruct& obj)
{
    QStructType type = obj.staticType;
    return qstructToJson(&obj,type);
}


template<QStruct TStruct>
void Converter::jsonToQStruct(void* obj,const std::string& source)
{
    QStructType type = TStruct::staticType;

    json::Parser parser {};
    auto ParserRes = parser.parse(source);
    if (auto* json = std::get_if<json::Object>(&ParserRes); json )
    {
        jsonToQStruct_inner(*json,type,obj);
    }
}
