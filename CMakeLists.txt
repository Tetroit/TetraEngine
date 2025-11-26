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
        "core/applications/*.h"
        "core/applications/*.cpp"
        "core/applications/**/*.h"
        "core/applications/**/*.cpp"

        "core/rendering/*.h"
        "core/rendering/*.cpp"

        "core/utils/*.h"
        "core/utils/*.cpp"

        "core/behaviours/*.h"
        "core/behaviours/*.cpp"

        "core/ecs/*.h"
        "core/ecs/*.cpp"

        "core/UI/*.h"
        "core/UI/*.cpp"

        "core/physics/*.h"
        "core/physics/*.cpp"

        core/pch/tetrapc.h
        core/pch/tetrapc.cpp

        core/TetraEngine.cpp

        core/FreeType.cpp
        core/FreeType.h

        core/Application.cpp
        core/Application.h
        core/Transform.cpp
        core/Transform.h
        core/GameObject.cpp
        core/GameObject.h
        core/DestroyManager.cpp
        core/DestroyManager.h
        core/ConsoleManager.cpp
        core/ConsoleManager.h
        core/Core.cpp
        core/Core.h
        core/Behaviour.cpp
        core/Behaviour.h
        core/InputManager.cpp
        core/InputManager.h
        core/GameObject_old.cpp
        core/GameObject_old.h
        core/GLFWManager.cpp
        core/GLFWManager.h
        core/config.h
        core/Test.cpp
        core/Test.h
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
        core/ecs/Handle.h
        core/ecs/ECS.cpp
        core/ecs/ECS.h
        core/ecs/Storage.h
)
target_link_libraries(UnitTest PRIVATE glfw glm assimp freetype imgui stb glad
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysX_static_64.lib"
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysXFoundation_static_64.lib"
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysXExtensions_static_64.lib"
)

target_include_directories(UnitTest PRIVATE
        ${CMAKE_SOURCE_DIR}/deps/assimp/include
        ${CMAKE_SOURCE_DIR}/core/pch
        ${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/include
)





project(TetraEngine)

add_executable(TetraEngine ${TETRA_ENGINE_GLOB}
        core/rendering/ViewProvider.cpp
        core/rendering/ViewProvider.h
        core/rendering/Camera.cpp
        core/rendering/Camera.h)

target_link_libraries(TetraEngine PRIVATE glfw glm assimp freetype imgui stb glad
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysX_static_64.lib"
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysXFoundation_static_64.lib"
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysXCommon_static_64.lib"
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysXPvdSDK_static_64.lib"
        "${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/bin/win.x86_64.vc143.md/debug/PhysXExtensions_static_64.lib"
)

target_include_directories(TetraEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/deps/assimp/include
        ${CMAKE_SOURCE_DIR}/core/pch
        ${CMAKE_SOURCE_DIR}/deps/PhysX_build/vs17cpu/include
)

target_precompile_headers(TetraEngine PUBLIC core/pch/tetrapc.h)

add_definitions(-DUNICODE -D_UNICODE)