#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "widget.hpp"

#include "dependency/raygui.h"
#include "overload.hpp"
#include "Panel.hpp"

#define QCLASS_DEF(name) class name : public Widget


QCLASS_DEF(PropertyViewer)
{
    PropertyViewer() { qStructType = &staticType; }
    ~PropertyViewer() override = default;

    WeakPtr<Widget> objectToShowWeak {};
    //void* objectToShow {nullptr};
    const QStructType* typeToShow {nullptr};

    WeakPtr<Container> container {};

    void onDraw(const Vector2& offset) override
    {
        float sliderValue;
        if(!typeToShow)
            return;
        void* obj =objectToShowWeak.get();
        if(!obj)
            return;

        float offsetY = offset.y+5;
        for( auto& fieldIt : typeToShow->getAllFields())
        {
            GuiLabel( (Rectangle){ offset.x+5, offsetY, 50, 20 }, fieldIt.name.c_str());

            std::visit(qstd::overloaded {
                    [&](const FieldType::Unimplemented& asUnimplemented)  {
                    },
                    [&](const FieldType::Float& asFloat)  {
                        auto* addr = fieldIt.getValuePtr<float>(obj);
                        GuiSlider((Rectangle){ offset.x+55, offsetY, 150, 20 }, nullptr, TextFormat("%2.2f", *addr), addr, 0, 1000);
                        *addr = int(*addr) / 10 * 10;
                    },
                    [&](const FieldType::StdString& asString)  {
                        auto val = fieldIt.getValuePtr<std::string>(obj);

                        int bufferSize = val->length() + 16;
                        char* valChars = new char[bufferSize];
                        strcpy(valChars,val->c_str());

                        GuiTextBox((Rectangle){ offset.x+55, offsetY, 150, 20 }, valChars, bufferSize,false);
                        *val = std::string(valChars);
                        delete[] valChars;
                    },
                    [&](const FieldType::QStruct& asQStruct)  {
                        //PropertyViewer inner{};
                        //inner.objectToShow  = static_cast<Widget*>(fieldIt.getValuePtr<void>(obj));
                        //inner.typeToShow  = asQStruct.type;
                        //inner.onDraw({offset.x+10,offsetY+25});
                    },
                    [&](const FieldType::QStructTypePtr& asQStructTypePtr)  {
                        //PropertyViewer inner{};
                        //inner.objectToShow  = static_cast<Widget*>(fieldIt.getValuePtr<void>(obj));
                        //inner.typeToShow  = asQStruct.type;
                        //inner.onDraw({offset.x+10,offsetY+25});
                    },
                    [&](const FieldType::OwnerPtr& asOwnerPtr)  {
                        auto& inner_type = *asOwnerPtr.innerType;
                        auto ownerPtr = fieldIt.getValuePtr<OwnerPtr<void>>(obj);

                        auto innerAddr = reinterpret_cast<void*>(ownerPtr->get());
                        if(!innerAddr)
                        {
                            GuiLabel( (Rectangle){ offset.x+25, offsetY+20, 50, 20 }, "nullptr");
                            return;
                        }

                        auto real_type = inner_type.getDynamicType(innerAddr);

                        if(GuiButton( (Rectangle){ offset.x+25, offsetY+20, 50, 20 }, "element"))
                        {
                            OwnerPtr<PropertyViewer>  viewer(new PropertyViewer());
                            viewer.get()->typeToShow = real_type;
                            viewer.get()->objectToShowWeak = WeakPtr<Widget>(*ownerPtr);

                            OwnerPtr<Panel> panel(new Panel);
                            panel.get()->widgets.push_back(std::move(viewer));
                            panel.get()->width = 400;
                            panel.get()->height = 400;

                            container.get()->widgetsToAdd.push_back(std::move(panel));
                        }
                    },
                    [&](const FieldType::WeakPtr& asWeakPtr)  {
                        auto& inner_type = *asWeakPtr.innerType;
                        auto weakPtr = fieldIt.getValuePtr<WeakPtr<void>>(obj);

                        auto innerAddr = reinterpret_cast<void*>(weakPtr->get());
                        if(!innerAddr)
                        {
                            GuiLabel( (Rectangle){ offset.x+25, offsetY+20, 50, 20 }, "weak nullptr");
                            return;
                        }

                        auto real_type = inner_type.getDynamicType(innerAddr);

                        if(GuiButton( (Rectangle){ offset.x+25, offsetY+20, 50, 20 }, "weak element"))
                        {
                            OwnerPtr<PropertyViewer>  viewer(new PropertyViewer());
                            viewer.get()->typeToShow = real_type;
                            viewer.get()->objectToShowWeak = *reinterpret_cast<WeakPtr<Widget>*>(weakPtr);

                            OwnerPtr<Panel> panel(new Panel);
                            panel.get()->widgets.push_back(std::move(viewer));
                            panel.get()->width = 400;
                            panel.get()->height = 400;

                            container.get()->widgetsToAdd.push_back(std::move(panel));
                        }
                    },
                    [&](const FieldType::DynamicArray& asDynamicArray)
                    {
                        offsetY += 25;

                        auto& valueType = *asDynamicArray.innerType;
                        void* fieldAddr = fieldIt.getValuePtr<void>(obj);
                        qstd::DynamicArrayScriptHelper helper { fieldAddr, getSizeOf(valueType) };

                        json::Array arr {};
                        for(size_t idx=0; idx != helper.get_length(); idx++)
                        {
                            void* addr = helper.get_elementPtr(idx);
                            GuiLabel( (Rectangle){ offset.x+25, offsetY, 50, 20 }, std::to_string(idx).c_str());
                            GuiLabel( (Rectangle){ offset.x+75, offsetY, 50, 20 }, "element");

                            if (auto* asOwnerPtrEl = std::get_if<FieldType::OwnerPtr>(&valueType))
                            {
                                auto inner_type = asOwnerPtrEl->innerType;
                                auto ownerPtr = reinterpret_cast<OwnerPtr<void>*>(addr);

                                auto innerAddr = reinterpret_cast<void*>(ownerPtr->get());
                                inner_type = inner_type->getDynamicType(innerAddr);


                                if(GuiButton( (Rectangle){ offset.x+75, offsetY, 200, 20 }, "element"))
                                {
                                    OwnerPtr<PropertyViewer>  viewer(new PropertyViewer());
                                    viewer.get()->typeToShow = inner_type;
                                    viewer.get()->objectToShowWeak = WeakPtr<Widget>(*ownerPtr);
                                    viewer.get()->container = container;

                                    OwnerPtr<Panel> panel(new Panel);
                                    panel.get()->widgets.push_back(std::move(viewer));
                                    panel.get()->width = 400;
                                    panel.get()->height = 400;

                                    container.get()->widgetsToAdd.push_back(std::move(panel));
                                }
                            }
                            else
                                GuiLabel( (Rectangle){ offset.x+75, offsetY, 200, 20 }, "element");

                            offsetY += 25;
                        }
                    }
            },fieldIt.type);

            offsetY += 25;
        }
    };
};

//TODO: make generate by Header Tool
GEN_QSTRUCT_TYPE(PropertyViewer,{
    GEN_QSTRUCT_FIELD_ENTRY(PropertyViewer,objectToShowWeak),
    GEN_QSTRUCT_FIELD_ENTRY(PropertyViewer,typeToShow),
    GEN_QSTRUCT_FIELD_ENTRY(PropertyViewer,container)
});