cmake_minimum_required(VERSION 3.16)

project(TetraEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(TetraEngine
        core/tests/HelloWorld.cpp
)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/assimp)
add_subdirectory(deps/freetype/freetype)

target_link_libraries(TetraEngine PRIVATE glfw glm assimp)

target_include_directories(TetraEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/deps/assimp/include
)