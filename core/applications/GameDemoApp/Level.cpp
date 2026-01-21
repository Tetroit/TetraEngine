#include "Level.h"

#include "Star.h"
#include "../GameObject.h"
#include "../../physics/Collider.h"
#include "../../rendering/VertexData.h"
#include "../../rendering/Shader.h"
#include "../../rendering/Scene.h"
#include "../../rendering/Material.h"
#include "../../utils/OBJParser.h"

using namespace TetraEngine;
namespace GameDemo {
    void Level::RegisterCallbacks(GameObject *obj) {
        obj->GetDestroyAction()->AddCallback([this](GameObject* arg) {
            this->DestroyCallback(arg);
        }, "Level");
    }
    void Level::DeregisterCallbacks(GameObject *obj) {
        obj->GetDestroyAction()->RemoveCallback("Level");
    }

    void Level::DestroyCallback(GameObject *obj) {
        auto it = std::find(objects.begin(), objects.end(), obj);
        DeregisterCallbacks(*it);
        if (it != objects.end()) {
            objects.erase(it);
        }
    }


    Level::Level(Scene* scene, Shader* shader) : scene(scene), litShader(shader) {
        triggerBox = new Collider();
        triggerBox->AddBox(0.5,0.5,0.5);
        triggerBox->MakeTrigger(true);

        starMaterial = new Material("Star");
        starMaterial->ambient = glm::vec3(0.1, 0.07, 0.0);
        starMaterial->diffuse = glm::vec3(0.9, 0.7, 0.0);
        starMaterial->specular = glm::vec3(0.7, 0.5, 0.3);

        blockMaterial = new Material("Block");
        blockMaterial->ambient = glm::vec3(0.1, 0.1, 0.1);
        blockMaterial->diffuse = glm::vec3(0.7, 0.7, 0.7);
        blockMaterial->specular = glm::vec3(0.5, 0.5, 0.5);

        AddBlock(glm::vec3(5, -3, 5),
            glm::vec3(0, 0, 0.5),
            glm::vec3(10, 1, 2));

        AddBlock(glm::vec3(15, -0.5, 5),
            glm::vec3(0, 0, 0),
            glm::vec3(10, 1, 2));

        AddBlock(glm::vec3(25, -3, 5),
            glm::vec3(0, 0, -0.5),
            glm::vec3(10, 1, 2));
    }

    void Level::AddBlock(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
        TETRA_USE_MAIN_ECS
        GameObject* block = new GameObject("Block");
        block->GetTransform()->SetPosition(pos);
        block->GetTransform()->SetRotation(rot);
        block->GetTransform()->SetScale(scale);
        objects.push_back(block);
        auto mrH = block->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), litShader);
        auto mr = ecs.GetComponent(mrH);
        mr->material = blockMaterial;

        auto rbH = block->AddComponent<RigidBody>(true);
        auto rb = ecs.GetComponent(rbH);

        auto* collider = new Collider();
        collider->AddBox(scale.x/2, scale.y/2, scale.z/2);
        colliders.push_back(collider);
        rb->SetCollider(collider);

        scene->AddObject(*block);
    }
    GameObject* Level::AddCubeTrigger(glm::vec3 pos, Shader* shader, Material* material, std::string name) {

        TETRA_USE_MAIN_ECS
        objects.push_back(new GameObject(name));
        GameObject* cubeObject = objects.back();
        RegisterCallbacks(cubeObject);
        cubeObject->GetTransform()->SetPosition(pos);
        // auto mrH = cubeObject->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), shader);
        // auto mr = ecs.GetComponent(mrH);
        // mr->material = material;
        auto rbH = cubeObject->AddComponent<RigidBody>(true);
        auto rb = ecs.GetComponent(rbH);
        rb->SetCollider(triggerBox);
        scene->AddObject(*cubeObject);
        return cubeObject;
    }

    Star* Level::AddStar(glm::vec3 pos, std::string name) {
        TETRA_USE_MAIN_ECS
        GameObject* starContainer = AddCubeTrigger(glm::vec3(pos), litShader, starMaterial, name);
        GameObject* starObject = new GameObject(name);
        Transform::SetParent(starObject->GetTransformHandle(), starContainer->GetTransformHandle());

        if (starModelID == -1) {
            starModelID = OBJParser::OBJRead(meshPath + "/star.obj");
        }
        auto mrH = starObject->AddComponent<MeshRenderer>(VertexData::GetPrefab(starModelID), litShader);
        auto mr = ecs.GetComponent(mrH);
        mr->material = starMaterial;
        return new Star(starContainer, starObject);
    }

    void Level::SetLitShader(Shader *shader) {
        litShader = shader;
    }

    Level::~Level() {
        for (int i=objects.size()-1; i>=0; i--) {
            DeregisterCallbacks(objects[i]);
            delete objects[i];
        }
        objects.clear();
    }
} // GameDemo