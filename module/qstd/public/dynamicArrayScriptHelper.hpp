#pragma once
#include <memory>
#include "dynamicArray.hpp"

namespace qstd
{
    struct DynamicArrayScriptHelper
    {
        void* fixedArrayPtr;
        const size_t value_size;

        size_t get_length();
        void reserve(size_t in_capacity);

        void* get_elementPtr(size_t in_index);
    };
}