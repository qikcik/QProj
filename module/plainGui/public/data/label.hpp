#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "widget.hpp"

struct Label : public Widget
{
    using base_class = Widget;
    static const QStructType staticType;

    Label() { qStructType = &staticType; }
    ~Label() override = default;

    std::string font {};
    std::string text {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Label,{
    GEN_QSTRUCT_FIELD_ENTRY(Label,font),
    GEN_QSTRUCT_FIELD_ENTRY(Label,text)
});