#pragma once

#include <string>
#include <cstdint>

enum class FieldType
{
    Unimplemented,Float,StdString
};

template<typename T>
FieldType getType();


struct FieldInfo
{
public:
    const std::string name {};
    const size_t offset {};
    const FieldType type {FieldType::Unimplemented};

    template<typename T>
    T& getValueRef(void* objPtr) const
    {
        auto* bytePtr = reinterpret_cast<uint8_t*>(objPtr);
        auto memberPtr = reinterpret_cast<T*>(bytePtr + offset);
        return *memberPtr;
    }
};