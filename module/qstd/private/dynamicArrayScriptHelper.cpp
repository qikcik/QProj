#include "dynamicArrayScriptHelper.hpp"

using namespace qstd;

size_t DynamicArrayScriptHelper::get_length()
{
    auto ptr = reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    return ptr->get_length();
}


void* DynamicArrayScriptHelper::get_elementPtr(size_t in)
{
    auto& ptr = *reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    auto* arr = ptr.array;

    return arr + (in * value_size);
}

void DynamicArrayScriptHelper::reserve(size_t in_capacity)
{
    auto& ptr = *reinterpret_cast<DynamicArray<uint8_t>*>(fixedArrayPtr);
    ptr.reserve(in_capacity,value_size);
}
