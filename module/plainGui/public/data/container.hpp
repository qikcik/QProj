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
    qstd::DynamicArray<OwnerPtr<Widget>> widgetsToAdd {};

    void onDraw(const Vector2& offset) override
    {
        for(auto& it : widgets)
        {
            if (it.get())
                it.get()->onDraw(offset);
        }
        for(auto& it : widgetsToAdd)
        {
            if (!it.get()) continue;

            it.get()->onDraw(offset);
            widgets.push_back(std::move(it));
        }
        widgetsToAdd.clear();

    };
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Container,{
    GEN_QSTRUCT_FIELD_ENTRY(Container,widgets),
    GEN_QSTRUCT_FIELD_ENTRY(Container,widgetsToAdd)
});