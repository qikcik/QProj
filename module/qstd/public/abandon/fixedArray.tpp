#pragma once

#include "fixedArray.hpp"

namespace qstd
{

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>::FixedArray()
    : length(0), array()
    {

    }

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>::~FixedArray()
    {

    }

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>::FixedArray(const FixedArray& other)
    {
        for(auto it : other)
        {
            push_back(std::move(it));
        }
    }

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>::FixedArray(FixedArray&& other) noexcept
    {
        for(TType& it : other)
        {
            push_back(std::move(it));
        }
    }

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>& FixedArray<TType, TLength>::operator=(const FixedArray& other)
    {
        if(this == &other)
            return *this;

        this->~FixedArray<TType, TLength>();

        std::allocator<FixedArray<TType, TLength>>().construct(this,other);
        return *this;
    }

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>& FixedArray<TType, TLength>::operator=(FixedArray&& other) noexcept
    {
        if(this == &other)
            return *this;

        this->~FixedArray<TType, TLength>();
        std::allocator<FixedArray<TType, TLength>>().construct(this,std::move(other));
        return *this;
    }

    template<typename TType, size_t TLength>
    const FixedArray<TType, TLength>::element_type&
    FixedArray<TType, TLength>::operator[](size_t in_position) const
    {
        return array[in_position];
    }

    template<typename TType, size_t TLength>
    FixedArray<TType, TLength>::element_type&
    FixedArray<TType, TLength>::operator[](size_t in_position)
    {
        return array[in_position];
    }

    template<typename TType, size_t TLength>
    bool FixedArray<TType, TLength>::has_freeSpace() const
    {
        return length != FixedArray<TType, TLength>::max_length;
    }

    template<typename TType, size_t TLength>
    void FixedArray<TType, TLength>::push_back(FixedArray<TType, TLength>::element_type&& in_element) noexcept
    {
        array[length] = std::move(in_element);
        length++;
    }

    template<typename TType, size_t TLength>
    size_t FixedArray<TType, TLength>::get_length() const
    {
        return length;
    }
}