cmake_minimum_required(VERSION 4.0)

set(VCPKG_APPLOCAL_DEPS ON)

project(projectV VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(OpenGL REQUIRED) # No CONFIG here
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


add_library(ProjectWarnings INTERFACE)

target_compile_options(ProjectWarnings INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)


add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Sandbox)


if(WIN32)
    target_link_libraries(Engine
    PUBLIC
        opengl32
        glu32
    )
endif()

target_link_libraries(Engine PUBLIC ProjectWarnings)
target_link_libraries(Editor PUBLIC ProjectWarnings)
target_link_libraries(Sandbox PUBLIC ProjectWarnings)

function(copy_resources TARGET)

    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:${TARGET}>/Resources
    )

endfunction()

# Enable message about build configuration
message(STATUS "Configured ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
