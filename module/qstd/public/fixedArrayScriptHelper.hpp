#pragma once
#include <memory>
#include "fixedArray.hpp"

namespace qstd
{
    struct FixedArrayScriptHelper
    {
        void* fixedArrayPtr;
        const size_t value_size;

        size_t get_length();
        void set_length(size_t in);
        void* get_elementPtr(size_t in);
    };
}