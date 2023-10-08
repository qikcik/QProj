#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "dependency/raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include <iostream>
#include "data/list.hpp"
#include "converter.hpp"
#include <fstream>
#include <sstream>

int main()
{
    QDynamicInfo dynamicInfo;
    int screenWidth = 400;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "ToDoAppSimple");
    SetExitKey(0);

    char fileName[128] = "default";
    bool fileNameEditMode = false;

    char newEntry[128] = "";
    bool newEntryEditMode = false;

    bool exitWindow = false;
    List list {};
    while (!exitWindow)
    {
        exitWindow = WindowShouldClose();


        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (GuiButton((Rectangle){ 10, 10, 70, 30 },  "Clear"))
            {
                list = {};
            }

            if (GuiTextBox((Rectangle){ 90, 10, 140, 30 }, fileName, 64, fileNameEditMode)) fileNameEditMode = !fileNameEditMode;

            if (GuiButton((Rectangle){ 240, 10, 70, 30 },  "Open"))
            {
                std::ifstream file(std::string(fileName) + ".json");
                std::stringstream stream {};
                stream << file.rdbuf();
                list = Converter(dynamicInfo).jsonToQStruct<List>(stream.str());
            }
            if (GuiButton((Rectangle){ 320, 10, 70, 30 },  "Save"))
            {
                std::ofstream file {};
                file.open ( std::string(fileName) + ".json");
                file << Converter(dynamicInfo).qstructToJson(list).stringify();
                std::cout << Converter(dynamicInfo).qstructToJson(list).stringify();
            }

            float temp = 50;
            for(auto& entry : list.entries)
            {
                GuiLabel( (Rectangle){ 10, temp, 300, 20 }, entry.title.c_str());
                bool checkBox = entry.status == "done";
                GuiCheckBox((Rectangle){ 340, temp, 20, 20 }, "DONE", &checkBox);
                entry.status = checkBox ? "done" : "";
                temp += 30;
            }


            if (GuiTextBox((Rectangle){ 10, 560, 300, 30 }, newEntry, 64, newEntryEditMode)) newEntryEditMode = !newEntryEditMode;
            if (GuiButton((Rectangle){ 320, 560, 70, 30 },  "Add"))
            {
                list.entries.push_back( Entry{newEntry, "" } );
            }

        EndDrawing();

    }
    CloseWindow();
    return 0;
}