#pragma once

namespace GameDemo {
    class Star;
}

namespace GameDemo {
    class CollectManager;
}

namespace TetraEngine {
    class Collider;
    class GameObject;
    class Shader;
    class Scene;
}
using namespace TetraEngine;
namespace GameDemo {
    class Level {
        Collider* triggerBox;
        std::vector<Collider*> colliders;
        Scene* scene;
        std::vector<GameObject*> objects;

        void RegisterCallbacks(GameObject* obj);
        void DeregisterCallbacks(GameObject* obj);
        void DestroyCallback(GameObject* obj);

        void AddBlock(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

        Material* starMaterial;
        Material* blockMaterial;
        Shader* litShader;
        int starModelID = -1;
    public:
        Level(Scene* scene, Shader* shader);
        GameObject* AddCubeTrigger(glm::vec3 pos, Shader* shader, Material* material, std::string name = "Cube Trigger");
        Star* AddStar(glm::vec3 pos, std::string name = "Star");
        void SetLitShader(Shader* shader);
        ~Level();
    };
}
