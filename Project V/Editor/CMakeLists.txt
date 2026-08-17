add_library(Editor STATIC)

target_sources(Editor
PRIVATE
    source/Gui.cpp
    source/imgui.cpp
    source/imgui_demo.cpp
    source/imgui_draw.cpp
    source/imgui_tables.cpp
    source/imgui_widgets.cpp
    source/imgui_impl_glfw.cpp
    source/imgui_impl_opengl3.cpp

PUBLIC
    include/Editor/Gui.h
)

target_include_directories(Editor
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(Editor
PRIVATE
    Engine
)
