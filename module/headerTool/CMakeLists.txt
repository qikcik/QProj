add_executable(HeaderTool headerTool.cpp)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/headerTool_generated.h
        COMMAND HeaderTool ${CMAKE_CURRENT_BINARY_DIR}/headerTool_generated.h
        DEPENDS HeaderTool
)

add_executable(
        HeaderTool_exe
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        ${CMAKE_CURRENT_BINARY_DIR}/headerTool_generated.h
)

target_include_directories(HeaderTool_exe PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)
