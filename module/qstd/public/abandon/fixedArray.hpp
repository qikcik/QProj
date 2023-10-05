#pragma once
#include <memory>

namespace qstd
{
    template<typename TType,size_t TLength>
    class FixedArray
    {
    public:
        using element_type = TType;
        static constexpr size_t max_length = TLength;

        FixedArray();
        ~FixedArray();
        FixedArray(const FixedArray& other);
        FixedArray(FixedArray&& other) noexcept;
        FixedArray& operator=(const FixedArray& other);
        FixedArray& operator=(FixedArray&& other) noexcept;

        inline bool has_freeSpace() const;
        inline size_t get_length() const;
        void push_back(element_type&& in_element) noexcept;

        element_type& operator[](size_t in_position);
        const element_type& operator[](size_t in_position) const;

        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = TType;
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

        Iterator begin() const { return Iterator(array); }
        Iterator end()   const { return Iterator(array+max_length); }

    protected:
        size_t length = 0;
        mutable element_type array[max_length] = {};

        friend class FixedArrayScriptHelper;
    };
}

#include "fixedArray.tpp"