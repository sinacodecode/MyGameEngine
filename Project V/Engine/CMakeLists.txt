add_library(Engine STATIC)

target_sources(Engine
PRIVATE

    source/Core/ConfigValues.cpp
    source/Renderer/camera.cpp
    source/Renderer/Mesh.cpp
    source/Renderer/Model.cpp
    source/Renderer/Renderer.cpp
    source/Renderer/Render.cpp
    source/Renderer/shader_s.cpp
    source/Renderer/texture_loader.cpp
    source/Scene/SceneObject.cpp


PUBLIC

    include/Core/ConfigValues.h
    include/Core/InputFunctions.h
    include/Core/resource.h

    include/Renderer/camera.h
    include/Renderer/Light.h
    include/Renderer/Mesh.h
    include/Renderer/Model.h
    include/Renderer/Renderer.h
    include/Renderer/Render.h
    include/Renderer/shader_s.h
    include/Renderer/stb_image.h
    include/Renderer/Transformations.h

    include/Scene/Scene.h
    include/Scene/SceneObject.h
)

target_include_directories(Engine
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Engine
    PUBLIC
        OpenGL::GL
        glfw
        glm::glm
        assimp::assimp
        glad::glad
)