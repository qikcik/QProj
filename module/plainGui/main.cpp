#include "raylib.h"
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include "dependency/raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "dynamicArray.hpp"
#include "qstructType.hpp"
#include "Converter.hpp"
#include <fstream>
#include <sstream>

#include "data/container.hpp"
#include "data/label.hpp"
#include "qstructType.hpp"
#include "data/Button.hpp"
#include "data/PropertyViewer.hpp"
#include "data/Panel.hpp"

int main()
{
    QDynamicInfo dynamicInfo;
    dynamicInfo.registerDynamicType(&Widget::staticType);
    dynamicInfo.registerDynamicType(&Label::staticType);

    dynamicInfo.registerDynamicType(&Container::staticType);
    dynamicInfo.registerDynamicType(&Button::staticType);
    dynamicInfo.registerDynamicType(&PropertyViewer::staticType);
    dynamicInfo.registerDynamicType(&Panel::staticType);

    OwnerPtr<Container>  container(new Container(), uuids::uuid::from_string("66536f5a-7005-45fb-b738-b43c6a73426d").value() );
    container.get()->width = 0;
    container.get()->height = 0;


    std::ifstream file("layout.json");
    std::stringstream stream {};
    stream << file.rdbuf();
    Converter(dynamicInfo).jsonToQStruct(*container.get(),stream.str());
    auto tmp = container.get();
    std::cout << to_string(container.block->uuid);

    OwnerPtr<Panel> panel(new Panel);
//
//    OwnerPtr<PropertyViewer>  viewer(new PropertyViewer());
//    viewer.get()->typeToShow = &Container::staticType;
//    viewer.get()->objectToShowWeak = container;
//    viewer.get()->container = container;
//
//    panel.get()->widgets.push_back(std::move(viewer));
//    panel.get()->width = 600;
//    panel.get()->height = 600;
//    container.get()->widgets.push_back(std::move(panel));
//
//

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "ToDoAppSimple");
    GuiSetStyle(SCROLLBAR, ARROWS_VISIBLE, true);

    bool exitWindow = false;

    Rectangle scissorRec = { 0 };
    Vector2 scrool {0};

    while (!exitWindow)
    {
        exitWindow = WindowShouldClose();


        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        container.get()->onDraw(Vector2{0,0});
        EndDrawing();
    }
    CloseWindow();


    std::ofstream file2 {};
    file2.open ( "layout2.json");
    file2 << Converter(dynamicInfo).qstructToJson(*container.get()).stringify();

    return 0;
}