#pragma once
#include "CollectManager.h"
#include "../../ecs/ECS.h"

class GameDemoApp;

namespace TetraEngine {
    class GameObject;
    class RigidBody;
    class MeshRenderer;
}
namespace GameDemo {

    class Star : public Collectable, public IUpdatable{
        GameObject* innerObject;
        ECS::Handle<MeshRenderer> mr;
        GameDemoApp* updateRoot;
    public:
        Star(GameObject* container, GameObject* object);
        ~Star() override;
        void Update(float dt) override;
        void SetUpdateRoot(GameDemoApp* newManager);
    };
}
