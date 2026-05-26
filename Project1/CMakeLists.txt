cmake_minimum_required(VERSION 3.1)

project(Project1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED NO)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(glad CONFIG REQUIRED)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
)

set(PROJECT_SOURCES
    Main.cpp
    Camera.cpp
    ConfigValues.cpp
    Gui.cpp
    Mesh.cpp
    Model.cpp
    Render.cpp
    Renderer.cpp
    SceneObject.cpp
    shader_s.cpp
    texture_loader.cpp
)

set(PROJECT_HEADERS
    Camera.h
    ConfigValues.h
    Gui.h
    InputFunctions.h
    Light.h
    Mesh.h
    Model.h
    Render.h
    Renderer.h
    Scene.h
    SceneObject.h
    shader_s.h
    resource.h
)

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
	${PROJECT_HEADERS}
	${IMGUI_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/imgui
	${OPENGL_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw
    glm::glm
    assimp::assimp
    glad::glad
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        opengl32
        glu32
    )
endif()

if(MSVC)
    # MSVC specific options
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W3
        /MP           # Multi-processor compilation
        /permissive-  # Standards conformance
    )
    
    # Disable specific warnings (adjust as needed)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /wd4251  # DLL interface warning
        /wd4996  # Deprecated functions
    )
    
    # For debug builds
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        _CRT_SECURE_NO_WARNINGS
    )
    
elseif(UNIX)
    # Linux/Unix specific options
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
    target_link_libraries(${PROJECT_NAME}
        dl
        pthread
    )
endif()

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32
        _WINDOWS
        GLEW_STATIC
    )
endif()

if(WIN32)
    # Create a custom command to copy DLLs
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/assimp-vc143-mtd.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying DLLs to output directory"
    )
    
    # Copy image assets
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png
            ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
            ${CMAKE_CURRENT_SOURCE_DIR}/container2.png
            ${CMAKE_CURRENT_SOURCE_DIR}/container2_specular.png
            ${CMAKE_CURRENT_SOURCE_DIR}/matrix.jpg
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying image assets to output directory"
    )
    
    # Copy shader files
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/fshader.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/vshader.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/lightcubeshader.fs
            ${CMAKE_CURRENT_SOURCE_DIR}/lightcubeshader.vs
            ${CMAKE_CURRENT_SOURCE_DIR}/modelLoader.fs
            ${CMAKE_CURRENT_SOURCE_DIR}/modelLoader.vs
            ${CMAKE_CURRENT_SOURCE_DIR}/plane.fs
            ${CMAKE_CURRENT_SOURCE_DIR}/plane.vs
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders.fs
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders.vs
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying shader files to output directory"
    )
    
    # Copy imgui.ini if it exists
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying imgui.ini to output directory"
    )
endif()

# Enable message about build configuration
message(STATUS "Configured ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "GLM found: ${GLM_FOUND}")  # Added status message for GLM