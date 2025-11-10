//
// Created by pchyo on 08/09/2025.
//

#ifndef TETRAENGINE_LIBCONFIGTEST_H
#define TETRAENGINE_LIBCONFIGTEST_H

#include <iostream>

#define ASSIMP_TEST true
#define GLM_TEST true
#define FREETYPE_TEST true
#define GLFW_TEST true
#define IMGUI_TEST true
#define GLAD_TEST true
#define STB_IMAGE_TEST true
#define PHYSX_TEST true

#if GLAD_TEST
#include <glad/glad.h>
#endif

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

#if IMGUI_TEST
#include <imgui.h>
#endif

#if STB_IMAGE_TEST
#include <stb_image.h>
#endif

#if PHYSX_TEST
#include <PxPhysicsAPI.h>
#endif

namespace TetraEngine::Tests{
    class LibConfigTest {
    public:
        static void run() {

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

#if IMGUI_TEST
            std::cout << ImGui::GetVersion() << std::endl;
#endif

#if GLAD_TEST
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                std::cout << "Failed to initialize GLAD\n jk all good for this test" << std::endl;
            else
                std::cout << "GLAD initialized" << std::endl;
#endif

#if STB_IMAGE_TEST
            std::cout << STBI_VERSION << std::endl;
#endif


#if PHYSX_TEST

            physx::PxDefaultAllocator gAllocator;
            physx::PxDefaultErrorCallback gErrorCallback;
            physx::PxFoundation* gFoundation = NULL;
            gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
            std::cout << gFoundation << std::endl;

            PX_RELEASE(gFoundation);

#endif

            std::cout << "Hello World!\n";
            std::cin.get();
        }
    };
}





#endif //TETRAENGINE_LIBCONFIGTEST_H