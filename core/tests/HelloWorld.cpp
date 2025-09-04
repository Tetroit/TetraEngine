// #include  <tetrapc.h>

#include <iostream>

#define ASSIMP_TEST true
#define GLM_TEST false
#define FREETYPE_TEST false
#define GLFW_TEST false

#if ASSIMP_TEST
#include <assimp/Importer.hpp>
#endif

#if GLM_TEST
#include <glm/glm.hpp>
#endif

#if GLFW_TEST
#include <glfw/glfw3.h>
#endif

#if FREETYPE_TEST
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

int main() {

#if ASSIMP_TEST
    Assimp::Importer importer;
    std::cout << importer.GetImporterInfo(0) << std::endl;
#endif

#if GLM_TEST
    glm::vec3 pos = glm::vec3(1.1, 2.2, 3.3);
    std::cout << glm::distance(glm::vec3(0), glm::vec3(1.1, 2.2, 3.3)) << std::endl;
#endif

#if GLFW_TEST
    std::cout << GLFW_KEY_BACKSLASH << std::endl;
#endif

#if FREETYPE_TEST
    FT_Library  library;
    auto error = FT_Init_FreeType( &library );
    std::cout<<error<<std::endl;
#endif

    std::cout << "Hello World!\n";
    return 0;
}
