#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "widget.hpp"

#include "dependency/raygui.h"

struct Label : public Widget
{
    using base_class = Widget;
    static const QStructType staticType;

    Label() { qStructType = &staticType; }
    ~Label() override = default;

    std::string text {};
    std::string text2 {};

    void onDraw(const Vector2& offset) override
    {
        GuiLabel( (Rectangle){ x + offset.x, y + offset.y, width, height }, text.c_str());
    };
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Label,{
    GEN_QSTRUCT_FIELD_ENTRY(Label,text),
    GEN_QSTRUCT_FIELD_ENTRY(Label,text2)
});