//
// Created by pchyo on 18/01/2026.
//

#include "Star.h"

#include "../GameDemoApp.h"
#include "../../Core.h"
#include "../../GameObject.h"

Star::Star(GameObject *container, GameObject *object) :
Collectable(container, container->GetComponentHandle<RigidBody>()),
innerObject(object)
{
    TETRA_USE_MAIN_ECS
    mr = object->GetComponentHandle<MeshRenderer>();
    innerObject->GetTransform()->SetLocalRotation(glm::quat(glm::vec3(3.14/2, 0, 0)));
}

Star::~Star() {
    if (updateRoot != nullptr) {
        updateRoot->RemoveUpdatable(this);
    }
    if (innerObject != nullptr) {
        innerObject->Destroy();
    }
}

void Star::Update(float dt) {
    innerObject->GetTransform()->GlobalRotate(glm::quat(glm::vec3(0, dt,0)));
}

void Star::SetUpdateRoot(GameDemoApp *newManager) {
    updateRoot = newManager;
}
