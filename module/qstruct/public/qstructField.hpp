#pragma once

#include <string>
#include <cstdint>
#include <variant>
#include <memory>

class QStructType;

namespace FieldType
{
    struct Unimplemented;
    struct Float;
    struct StdString;
    struct QStruct;
    struct DynamicArray;

    using type = std::variant<Unimplemented,Float,StdString,QStruct,DynamicArray>;

    struct Unimplemented {

    };
    struct Float {

    };
    struct StdString {

    };
    struct QStruct {
        const QStructType* type;
    };
    struct DynamicArray {
        std::shared_ptr<FieldType::type> innerType; //TODO: fix copy behaviour and change to unique_ptr
    };

}

template<typename T>
FieldType::type getType();

size_t getSizeOf(const FieldType::type& ref);

struct FieldInfo
{
public:
    const std::string name {};
    const size_t offset {};
    const FieldType::type type {FieldType::Unimplemented{}};

    template<typename T>
    T& getValueRef(void* objPtr) const
    {
        auto* bytePtr = reinterpret_cast<uint8_t*>(objPtr);
        auto memberPtr = reinterpret_cast<T*>(bytePtr + offset);
        return *memberPtr;
    }
    template<typename T>
    T* getValuePtr(void* objPtr) const
    {
        auto* bytePtr = reinterpret_cast<uint8_t*>(objPtr);
        auto memberPtr = reinterpret_cast<T*>(bytePtr + offset);
        return memberPtr;
    }
};