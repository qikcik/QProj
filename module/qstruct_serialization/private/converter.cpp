#include "converter.hpp"
#include "dynamicArray.tpp"
#include "overload.hpp"

json::innerType Converter::FieldToJson(void* addr,const FieldType::type& type)
{
    return std::visit(qstd::overloaded {
        [&](const FieldType::Unimplemented& asUnimplemented) -> json::innerType {
            return json::Value("unimplemented");
        },
        [&](const FieldType::Float& asFloat) -> json::innerType  {
            auto& value = *reinterpret_cast<float*>(addr);
            return json::Value(value);
        },
        [&](const FieldType::StdString& asString) -> json::innerType  {
            auto& value = *reinterpret_cast<std::string*>(addr);
            return json::Value(value);
        },
        [&](const FieldType::QStruct& asQStruct) -> json::innerType  {
            auto& inner_type = *asQStruct.type;
            return Converter::qstructToJson(addr,inner_type);
        },
        [&](const FieldType::StdUniquePtr& asUniquePtr) -> json::innerType  {
            auto& inner_type = *asUniquePtr.innerType;
            auto uniquePtr = reinterpret_cast<std::unique_ptr<void>*>(addr);
            auto innerAddr = reinterpret_cast<void*>(uniquePtr->get());

            auto real_type = inner_type.getDynamicType(innerAddr);
            return Converter::qstructToJson(innerAddr,*real_type);
        },
        [&](const FieldType::DynamicArray& asDynamicArray) -> json::innerType  {
            auto& valueType = *asDynamicArray.innerType;
            qstd::DynamicArrayScriptHelper helper { addr, getSizeOf(valueType) };

            json::Array arr {};
            for(size_t idx=0; idx != helper.get_length(); idx++)
            {
                void* addr = helper.get_elementPtr(idx);
                arr.values.emplace_back(FieldToJson(addr, *asDynamicArray.innerType));
            }
            return arr;
        }
    },type);
}

json::Object Converter::qstructToJson(void* obj,const QStructType& inType)
{
    json::Object result {};
    const QStructType* actualType = &inType;

    if(inType.implType == QStructType::ImplType::Dynamic)
    {
        result.set("qStructType", json::Value( inType.getDynamicType(obj)->name ));
        actualType = inType.getDynamicType(obj);
    }

    for(const auto& fieldIt : actualType->getAllFields())
    {
        auto addr = fieldIt.getValuePtr<void>(obj);
        result.set(fieldIt.name,FieldToJson(addr,fieldIt.type));
    }

    return result;
}

void Converter::jsonArrayToQstruct(json::Array& arrayJson,const FieldType::DynamicArray& dynamicArray, void* addr)
{
    const auto& valueType = *dynamicArray.innerType;
    new (addr) qstd::DynamicArray<uint8_t>(arrayJson.values.size(), getSizeOf(valueType) );
    qstd::DynamicArrayScriptHelper helper { addr, getSizeOf(valueType) };
    helper.set_length(arrayJson.values.size());
    int i=0;
    for( auto& valueIt : arrayJson.values)
    {
        auto* elPtr = helper.get_elementPtr(i);
        std::visit(qstd::overloaded{
                [&](const FieldType::Unimplemented& type) {
                },
                [&](const FieldType::Float& type) {
                    auto asFloat = std::get<json::Value>(valueIt).get<float>();
                    *reinterpret_cast<float*>(elPtr) = asFloat;
                },
                [&](const FieldType::StdString& type) {
                    auto asString = std::get<json::Value>(valueIt).get<std::string>();
                    *reinterpret_cast<std::string*>(elPtr) = asString;
                },
                [&](const FieldType::QStruct& type) {
                    auto& inner_json = std::get<json::Object>(valueIt);
                    jsonToQStruct_inner(inner_json,*type.type,elPtr);
                },
                [&](const FieldType::StdUniquePtr& type) {
                    auto& inner_type = *type.innerType;
                    auto uniquePtr = reinterpret_cast<std::unique_ptr<void>*>(elPtr);
                    auto innerAddr = uniquePtr->get();

                    auto& inner_json = std::get<json::Object>(valueIt);
                    auto realTypeName = inner_json.get<json::Value>("qStructType").get<std::string>();
                    auto realType = dynamicInfo.getDynamicType(realTypeName);

                    void* newObj = new uint8_t[realType->size];
                    jsonToQStruct_inner(inner_json,*realType,newObj);
                    new (uniquePtr) std::unique_ptr<void>(newObj);
                },
                [&](const FieldType::DynamicArray& type) {
                    auto& inner_json = std::get<json::Array>(valueIt);
                    jsonArrayToQstruct(inner_json,type,elPtr);
                }
        },valueType);
        i++;
    }
}

void Converter::jsonToQStruct_inner(json::Object& json,const QStructType& inType, void* obj)
{
    const QStructType* actualType = &inType;
    if(inType.implType == QStructType::ImplType::Dynamic)
    {
        auto typeName = std::get<json::Value>(json.entries["qStructType"]).get<std::string>();
        actualType = dynamicInfo.getDynamicType(typeName);
    }

    actualType->initInstance(obj);

    for( auto& entryIt : json.entries )
    {
        if(entryIt.first == "qStructType")
            continue;

        auto field = actualType->getField(entryIt.first);
        std::visit(qstd::overloaded {
                [&](const FieldType::Unimplemented& asUnimplemented)  {
                },
                [&](const FieldType::Float& asFloat)  {
                    field.getValueRef<float>(obj) = std::get<json::Value>(entryIt.second).get<float>();
                },
                [&](const FieldType::StdString& asString)  {
                    void* value = field.getValuePtr<void>(obj);
                    new (value) std::string(std::move(std::get<json::Value>(entryIt.second).get<std::string>()));
                },
                [&](const FieldType::QStruct& asQStruct)  {
                    auto& inner_json = std::get<json::Object>(entryIt.second);
                    void* value = field.getValuePtr<void>(obj);
                    jsonToQStruct_inner(inner_json,*asQStruct.type,value);
                },
                [&](const FieldType::StdUniquePtr& asUniquePtr)  {
                    auto& inner_type = *asUniquePtr.innerType;
                    auto uniquePtr = field.getValuePtr<std::unique_ptr<void>>(obj);
                    auto innerAddr = uniquePtr->get();

                    if(innerAddr)
                    {
                        inner_type.deInitInstance(innerAddr);
                        delete innerAddr;
                    }

                    auto& inner_json = std::get<json::Object>(entryIt.second);
                    auto realTypeName = inner_json.get<json::Value>("qStructType").get<std::string>();
                    auto realType = dynamicInfo.getDynamicType(realTypeName);

                    void* newObj = new uint8_t[realType->size];
                    jsonToQStruct_inner(inner_json,*realType,newObj);
                    new (uniquePtr) std::unique_ptr<void>(newObj);
                },
                [&](const FieldType::DynamicArray& asDynamicArray)  {
                    void* addr = field.getValuePtr<void>(obj);
                    auto& arrayJson = std::get<json::Array>(entryIt.second);
                    jsonArrayToQstruct(arrayJson,asDynamicArray,addr);
                }
        },field.type);
    }
}