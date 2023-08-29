defineModule("qui_core")

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_PROGRESS TRUE
        )

FetchContent_MakeAvailable(SFML)

addDependency("qui_core" "sfml-graphics")
defineExecutableForModule("qui_core")
