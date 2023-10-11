#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "widget.hpp"

#include "dependency/raygui.h"

struct Button : public Widget
{
    using base_class = Widget;
    static const QStructType staticType;

    Button() { qStructType = &staticType; }
    ~Button() override = default;

    std::string text {};

    void onDraw(const Vector2& offset) override
    {
        GuiButton( (Rectangle){ x + offset.x, y + offset.y, width, height }, text.c_str());
    };
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Button,{
    GEN_QSTRUCT_FIELD_ENTRY(Button,text)
});