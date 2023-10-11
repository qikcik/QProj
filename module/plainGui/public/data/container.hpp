#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "widget.hpp"
#include "DynamicArray.tpp"

struct Container : public Widget
{
    using base_class = Widget;
    static const QStructType staticType;

    Container() { qStructType = &staticType; }


    ~Container() override = default;

    qstd::DynamicArray<OwnerPtr<Widget>> widgets {};

    void onDraw(const Vector2& offset) override
    {
        for(auto& it : widgets)
            if(it.get())
                it.get()->onDraw(offset);
    };
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Container,{
    GEN_QSTRUCT_FIELD_ENTRY(Container,widgets)
});