#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"

struct Widget
{
    static const QStructType staticType;
    const QStructType* qStructType = &Widget::staticType;
    virtual ~Widget() = default;

    float x {};
    float y {};
    float width {};
    float height {};

    virtual void onDraw() {};
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Widget,{
    GEN_QSTRUCT_FIELD_ENTRY(Widget,x),
    GEN_QSTRUCT_FIELD_ENTRY(Widget,y),
    GEN_QSTRUCT_FIELD_ENTRY(Widget,width),
    GEN_QSTRUCT_FIELD_ENTRY(Widget,height),
});