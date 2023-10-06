#pragma once
#include "dynamicArray.hpp"

namespace qstd
{
    template<typename TType>
    DynamicArray<TType>::DynamicArray(size_t in_capacity, size_t in_elementSize)
    : capacity(in_capacity), length(0)
    {
        reserve_impl(capacity,in_elementSize);
    }

    template<typename TType>
    DynamicArray<TType>::~DynamicArray()
    {
        if(array != nullptr)

            delete[] reinterpret_cast<uint8_t*>(array);
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
        new (this) DynamicArray<TType>(other);
        return *this;
    }

    template<typename TType>
    DynamicArray<TType>& DynamicArray<TType>::operator=(DynamicArray&& other) noexcept
    {
        if(this == &other)
            return *this;

        this->~DynamicArray<TType>();
        new (this) DynamicArray<TType>(std::move(other));
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
        return array[in_position];
    }

    template<typename TType>
    DynamicArray<TType>::element_type&
    DynamicArray<TType>::operator[](size_t in_position)
    {
        return array[in_position];
    }

    template<typename TType>
    void DynamicArray<TType>::push_back(element_type&& in_element) noexcept
    {
        if (length == capacity)
            reserve(capacity * 2);

        if constexpr ( std::is_same_v<float, TType> )
            array[length] = in_element;
        else
            TType* ptr = &array[length];
            new (&array[length]) TType( std::move(in_element) );
        length++;
    }

    template<typename TType>
    void DynamicArray<TType>::reserve(size_t in_capacity) noexcept
    {
        if(in_capacity < capacity)
            return;

        reserve_impl(in_capacity,sizeof(TType));
    }


    template<typename TType>
    void DynamicArray<TType>::reserve_impl(size_t in_capacity, size_t in_elementSize) noexcept
    {
        element_type* old = array;
        capacity = in_capacity;
        array = reinterpret_cast<TType*>(new uint8_t[capacity * in_elementSize]);
        std::copy(old, old + length, array); // foreach with move constructor, would be a better option
        delete[] old;
    }
}