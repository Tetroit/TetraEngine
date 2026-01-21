#include "PlayerController.h"

#include "../../Core.h"
#include "../../physics/RigidBody.h"

namespace GameDemo {
    PlayerController::PlayerController(ECS::Handle<RigidBody> rb) {
        rigidBody = rb;
        camera = ECS::Handle<Transform>();
        TETRA_USE_GAME_INPUT
        input->AddListener(AnyKeyEvent::GetLink(GLFW_REPEAT), &PlayerController::OnKeyPress, *this);
    }

    PlayerController::~PlayerController() {
        TETRA_USE_GAME_INPUT
        input->RemoveListener(AnyKeyEvent::GetLink(GLFW_PRESS), &PlayerController::OnKeyPress, *this);
    }

    void PlayerController::OnKeyPress(const Event<InputEvent> &ev) {
    }

    void PlayerController::Update(float dt) {
        TETRA_USE_MAIN_ECS
        velocity = glm::vec2(0);
        bool noInput = true;
        if (Core::GetInputManager()->IsKeyDown(GLFW_KEY_W)) {
            velocity+=glm::vec2(0.0f, 1.0f);
            noInput = false;
        }
        if (Core::GetInputManager()->IsKeyDown(GLFW_KEY_A)) {
            velocity+=glm::vec2(-1.0f, 0.0f);
            noInput = false;
        }
        if (Core::GetInputManager()->IsKeyDown(GLFW_KEY_S)) {
            velocity+=glm::vec2(0.0f, -1.0f);
            noInput = false;
        }
        if (Core::GetInputManager()->IsKeyDown(GLFW_KEY_D)) {
            velocity+=glm::vec2(1.0f, 0.0f);
            noInput = false;
        }
        if (noInput) {
            return;
        }
        velocity = normalize(velocity);
        auto* rb = ecs.GetComponent(rigidBody);
        rb->AddLinear(glm::vec3(-velocity.x, 0.0f, velocity.y), PxForceMode::eACCELERATION);
    }
} // GameDemo