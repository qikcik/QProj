defineModule("todoApp")

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib
        GIT_TAG master
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)


addDependency("todoApp" "raylib")
addModuleDependency("todoApp" "qstruct_serialization")
defineExecutableForModule("todoApp")
