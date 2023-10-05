#include "abandon/fixedArrayScriptHelper.hpp"

using namespace qstd;

size_t FixedArrayScriptHelper::get_length()
{
    auto ptr = reinterpret_cast<FixedArray<uint8_t, 1>*>(fixedArrayPtr);
    return ptr->get_length();
}

void FixedArrayScriptHelper::set_length(size_t in)
{
    auto ptr = reinterpret_cast<FixedArray<uint8_t, 1>*>(fixedArrayPtr);
    ptr->length = in;
}

void* FixedArrayScriptHelper::get_elementPtr(size_t in)
{
    auto& ptr = *reinterpret_cast<FixedArray<uint8_t, 1>*>(fixedArrayPtr);
    auto* arr = ptr.array;

    return arr + (in * value_size);
}