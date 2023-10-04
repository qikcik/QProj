#pragma once
#include "dynamicArray.hpp"

namespace qstd
{
    template<typename TType>
    DynamicArray<TType>::DynamicArray(size_t in_capacity)
    : capacity(0), length(0)
    {
        reserve(capacity);
    }

    template<typename TType>
    DynamicArray<TType>::~DynamicArray()
    {
        if(array != nullptr)
            delete array[];
    }


    template<typename TType>
    DynamicArray<TType>::DynamicArray(const DynamicArray& other)
    : capacity(0), length(0)
    {
        reserve(other.capacity);
        for(TType it : other)
        {
            push_back(std::move(it));
        }
    }

    template<typename TType>
    DynamicArray<TType>::DynamicArray(DynamicArray&& other) noexcept
    : capacity(0), length(0)
    {
        reserve(other.capacity);
        for(TType& it : other)
        {
            push_back(std::move(it));
        }

        other.array = nullptr;
        other.capacity = 0;
        other.length = 0;
    }

    template<typename TType>
    DynamicArray<TType>& DynamicArray<TType>::operator=(const DynamicArray& other)
    {
        if(this == &other)
            return *this;

        this->~DynamicArray<TType>();
        std::allocator<DynamicArray<TType>>().construct(this,other);
        return *this;
    }

    template<typename TType>
    DynamicArray<TType>& DynamicArray<TType>::operator=(DynamicArray&& other) noexcept
    {
        if(this == &other)
            return *this;

        this->~DynamicArray<TType>();
        std::allocator<DynamicArray<TType>>().construct(this,std::move(other));
        return *this;
    }

    template<typename TType>
    size_t DynamicArray<TType>::get_length() const
    {
        return length;
    }

    template<typename TType>
    size_t DynamicArray<TType>::get_capacity() const
    {
        return capacity;
    }

    template<typename TType>
    const DynamicArray<TType>::element_type&
    DynamicArray<TType>::operator[](size_t in_position) const
    {
        return array+in_position;
    }

    template<typename TType>
    DynamicArray<TType>::element_type&
    DynamicArray<TType>::operator[](size_t in_position)
    {
        return array+in_position;
    }

    template<typename TType>
    void DynamicArray<TType>::push_back(element_type&& in_element) noexcept
    {
        if (length == capacity)
            reserve(capacity * 2);

        array[length++] = std::move(in_element);
    }

    template<typename TType>
    void DynamicArray<TType>::reserve(size_t in_capacity, size_t elementSize) noexcept
    {
        if(in_capacity <= capacity)
            return;

        element_type* old = array;
        capacity = in_capacity;
        array = new uint8_t[capacity*elementSize];
        copy(old, old + length, array);
        delete[] old;
    }
}