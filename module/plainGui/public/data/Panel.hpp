#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "widget.hpp"
#include "DynamicArray.tpp"
#include "raylib.h"
#include "dependency/raygui.h"

struct Panel : public Widget
{
    using base_class = Widget;
    static const QStructType staticType;

    Panel() { qStructType = &staticType; }


    ~Panel() override = default;

    qstd::DynamicArray<OwnerPtr<Widget>> widgets {};
    std::string tittle {};

    bool windowActive = true;
    bool dragMode = false;
    Vector2 panOffset = { 0, 0 };

    void onDraw(const Vector2& offset) override
    {
        if (windowActive)
        {
            Vector2 mousePosition = GetMousePosition();

            if (CheckCollisionPointRec(mousePosition, (Rectangle) {x, y, width, 24}) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                dragMode = true;
                panOffset.x = mousePosition.x - x;
                panOffset.y = mousePosition.y - y;
            }

            if (dragMode)
            {
                x = (mousePosition.x - panOffset.x);
                y = (mousePosition.y -panOffset.y);

                // Check screen limits to avoid moving out of screen
                if (x < 0) x = 0;
                else if (x > (GetScreenWidth() - width)) x = GetScreenWidth() - width;

                if (y < 0) y = 0;
                else if (y > (GetScreenHeight() - height)) y = GetScreenHeight() - height;

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragMode = false;
            }

            windowActive = !GuiWindowBox(Rectangle(x,y,width,height), tittle.c_str());

            BeginScissorMode(x, y+24, width, height-24);
                for (auto& it: widgets)
                    if(it.get())
                        it.get()->onDraw(Vector2(offset.x + x,offset.y + y+24));
            EndScissorMode();
        }
    };
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(Panel,{
    GEN_QSTRUCT_FIELD_ENTRY(Panel,widgets),
    GEN_QSTRUCT_FIELD_ENTRY(Panel,tittle)
});