#pragma once
#include <memory>

namespace qstd
{
    template<typename TType>
    class DynamicArray
    {
    public:
        using element_type = TType;

        explicit DynamicArray(size_t in_capacity = 8);

        ~DynamicArray();
        DynamicArray(const DynamicArray& other);
        DynamicArray(DynamicArray&& other) noexcept;
        DynamicArray& operator=(const DynamicArray& other);
        DynamicArray& operator=(DynamicArray&& other) noexcept;

        inline size_t get_length() const;
        inline size_t get_capacity() const;

        void reserve(size_t in_capacity,size_t elementSize = sizeof(TType)) noexcept;
        void push_back(element_type&& in_element) noexcept;

        element_type& operator[](size_t in_position);
        const element_type& operator[](size_t in_position) const;

        class Iterator;

        Iterator begin() { return Iterator(*array); }
        Iterator end()   { return Iterator(*array+length); }

    protected:
        size_t length = 0;
        size_t capacity = 0;
        element_type* array = {};

        friend class DynamicArrayScriptHelper;
    };

    template<typename TType>
    struct DynamicArray<TType>::Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = element_type;
        using pointer           = element_type*;
        using reference         = element_type&;

        Iterator(pointer in) : ptr(in) {}

        reference operator*() const { return *ptr; }
        pointer operator->() { return ptr; }
        Iterator& operator++() { ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };
    private:
        pointer ptr;
    };
}

#include "fixedArray.tpp"