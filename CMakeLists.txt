cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/assimp)
add_subdirectory(deps/freetype)

file(GLOB IMGUI_GLOB
        deps/imgui/imgui.cpp
        deps/imgui/imgui.h
        deps/imgui/imgui_demo.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_widgets.cpp
        deps/imgui/imgui_tables.cpp
        deps/imgui/imgui_internal.h
        deps/imgui/imstb_rectpack.h
        deps/imgui/imstb_textedit.h
        deps/imgui/imstb_truetype.h
        deps/imgui/imconfig.h

        deps/imgui/backends/imgui_impl_opengl3.cpp
        deps/imgui/backends/imgui_impl_opengl3.h
        deps/imgui/backends/imgui_impl_opengl3_loader.h
        deps/imgui/backends/imgui_impl_glfw.cpp
        deps/imgui/backends/imgui_impl_glfw.h)

file (GLOB GLAD_GLOB
        deps/glad/include/glad/glad.h
        deps/glad/src/glad.c)

file (GLOB STB_GLOB
        deps/stb_image/stb_impl.cpp
        deps/stb_image/stb_image.h)

file (GLOB TETRA_ENGINE_GLOB
        core/TetraEngine.cpp
        core/pch/tetrapc.h
        core/pch/tetrapc.cpp
        core/rendering/Texture2D.h
        core/rendering/Cubemap.cpp
        core/utils/Time.h
        core/ImGuiManager.cpp
        core/rendering/MeshRenderer.cpp
        core/FreeType.cpp
        core/FreeType.h
        core/registry/SharedContentRegistry.h
        core/Application.cpp
        core/rendering/PointLight.h
        core/utils/OBJParser.cpp
        core/rendering/LightManager.h
        core/rendering/Texture2D.cpp
        core/behaviours/Bullet.h
        core/rendering/Camera.cpp
        core/rendering/Light.h
        core/ConsoleManager.h
        core/utils/Utils.h
        core/rendering/Camera.h
        core/ecs/Component.h
        core/rendering/Viewport.h
        core/rendering/Shader.cpp
        core/rendering/Viewport.cpp
        core/MyApplication.h
        core/rendering/Framebuffer.h
        core/rendering/Skybox.cpp
        core/Core.h
        core/ImGuiManager.h
        core/DestroyManager.cpp
        core/rendering/MeshRenderer.h
        core/ecs/ComponentManager.cpp
        core/DestroyManager.h
        core/rendering/Shader.h
        core/rendering/LightRenderer.h
        core/behaviours/Enemy.h
        core/rendering/Material.h
        core/Behaviour.cpp
        core/InputManager.cpp
        core/rendering/Renderbuffer.cpp
        core/Test.cpp
        core/behaviours/Enemy.cpp
        core/ecs/ComponentManager.h
        core/rendering/PointLight.cpp
        core/registry/SharedContentRegistry.cpp
        core/rendering/Material.cpp
        core/rendering/Renderbuffer.h
        core/Test.h
        core/rendering/Scene.cpp
        core/behaviours/TestBehaviour.h
        core/GameObject.cpp
        core/rendering/Skybox.h
        core/utils/Event.h
        core/rendering/Texture360.h
        core/rendering/LightManager.cpp
        core/GLFWManager.h
        core/utils/Time.cpp
        core/rendering/Scene.h
        core/rendering/Cubemap.h
        core/InputManager.h
        core/rendering/Texture360.cpp
        core/Application.h
        core/MyApplication.cpp
        core/utils/OBJParser.h
        core/GameObject.h
        core/ConsoleManager.cpp
        core/behaviours/Bullet.cpp
        core/Behaviour.h
        core/rendering/VertexData.cpp
        core/Core.cpp
        core/config.h
        core/utils/Types.h
        core/rendering/Light.cpp
        core/ecs/Component.cpp
        core/GLFWManager.cpp
        core/rendering/LightRenderer.cpp
        core/rendering/VertexData.h
        core/rendering/Framebuffer.cpp
)

add_library(imgui STATIC ${IMGUI_GLOB})

target_link_libraries(imgui PRIVATE glfw)

target_include_directories(imgui
        PUBLIC
        deps/imgui
        PRIVATE
        deps/imgui/backends
)

add_library(stb STATIC ${STB_GLOB})

target_include_directories(stb PUBLIC
        ${CMAKE_SOURCE_DIR}/deps/stb_image
)

add_library(glad STATIC ${GLAD_GLOB})

target_include_directories(glad PUBLIC
        ${CMAKE_SOURCE_DIR}/deps/glad/include)





project(UnitTest)

add_executable(UnitTest
        core/tests/Test.cpp
        core/tests/LibConfigTest.h
        core/tests/ECSTest.h
        core/ecs/Component.cpp
        core/ecs/Component.h
        core/ecs/ComponentManager.cpp
        core/ecs/ComponentManager.h
        core/ecs/ComponentHandle.h
        core/ecs/ComponentHandle.inl
        core/ecs/Entity.cpp
        core/ecs/Entity.h
        core/ecs/Handle.cpp
        core/ecs/Handle.h
        core/ecs/ECS.cpp
        core/ecs/ECS.h
)
target_link_libraries(UnitTest PRIVATE glfw glm assimp freetype imgui stb glad)

target_include_directories(UnitTest PRIVATE
        ${CMAKE_SOURCE_DIR}/deps/assimp/include
        ${CMAKE_SOURCE_DIR}/core/pch
)





project(TetraEngine)

add_executable(TetraEngine ${TETRA_ENGINE_GLOB}
        core/ecs/ComponentHandle.h
        core/ecs/ComponentHandle.inl
        core/ecs/Entity.cpp
        core/ecs/Entity.h
        core/ecs/Storage.cpp
        core/ecs/Storage.h
        core/ecs/Handle.cpp
        core/ecs/Handle.h
        core/ecs/ECS.cpp
        core/ecs/ECS.h)

target_link_libraries(TetraEngine PRIVATE glfw glm assimp freetype imgui stb glad)

target_include_directories(TetraEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/deps/assimp/include
        ${CMAKE_SOURCE_DIR}/core/pch)

target_precompile_headers(TetraEngine PUBLIC core/pch/tetrapc.h)