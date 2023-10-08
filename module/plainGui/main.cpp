#include "raylib.h"

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
#include "data/button.hpp"
#include "qstructType.hpp"

int main()
{
    QDynamicInfo dynamicInfo;
    dynamicInfo.registerDynamicType(&Widget::staticType);
    dynamicInfo.registerDynamicType(&Label::staticType);
    dynamicInfo.registerDynamicType(&Button::staticType);
    dynamicInfo.registerDynamicType(&Container::staticType);

    Container window {};
//    window.width = 600;
//    window.height = 600;
//
//    auto label = std::make_unique<Label>();
//    label->x = 10;
//    label->y = 10;
//    label->width = 200;
//    label->height = 20;
//    label->text = "test";
//    window.widgets.push_back(std::move(label));
//

    std::ifstream file("layout.json");
    std::stringstream stream {};
    stream << file.rdbuf();

    Converter(dynamicInfo).jsonToQStruct<Container>(&window,stream.str());

    InitWindow(window.width, window.height, "ToDoAppSimple");
    bool exitWindow = false;
    while (!exitWindow)
    {
        exitWindow = WindowShouldClose();


        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            window.onDraw();
        EndDrawing();

    }
    CloseWindow();


    //std::ofstream file2 {};
    //file2.open ( "layout.json");
    //file2 << Converter(dynamicInfo).qstructToJson(window).stringify();

    return 0;
}