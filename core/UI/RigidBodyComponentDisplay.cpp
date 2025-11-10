
#include "RigidBodyComponentDisplay.h"
#include "../Core.h"
#include "../physics/RigidBody.h"

namespace TetraEngine::UI {
    void RigidBodyComponentDisplay::Draw(ECS::TypeErasedHandle handle) {
        TETRA_USE_MAIN_ECS
            auto transformHandle = static_cast<ECS::Handle<RigidBody>>(handle);
        RigidBody* rb = ecs.GetComponent<RigidBody>(transformHandle);
        if (ImGui::CollapsingHeader("Rigid Body")) {

            ImGui::PushID("RigidBody");
            bool isStatic = rb->IsStatic();
            float mass = rb->GetMass();
            glm::vec3 position = rb->GetPosition();
            glm::vec3 rotation = glm::eulerAngles(rb->GetRotation());
            glm::vec3 linearVelocity = rb->GetLinearVelocity();
            glm::vec3 angularVelocity = rb->GetAngularVelocity();
            glm::vec3 linearAcceleration = rb->GetLinearAcceleration();
            glm::vec3 angularAcceleration = rb->GetAngularAcceleration();

            if (ImGui::Checkbox("Static", &isStatic)) {
                if (isStatic) {
                    rb->SetStatic();
                }
                else {
                    rb->SetDynamic();
                }
            }

            if (ImGui::DragFloat("Mass", &mass)) {
                rb->SetMass(mass);
            }

            if (ImGui::DragFloat3("Position", &position.x, 0.1f)) {
                rb->SetPosition(position);
            }
            if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) {
                 rb->SetRotation(rotation);
            }
            if (ImGui::DragFloat3("Linear Velocity", &linearVelocity.x, 0.1f)) {
                rb->SetLinearVelocity(linearVelocity);
            }
            if (ImGui::DragFloat3("Angular Velocity", &angularVelocity.x, 0.1f)) {
                rb->SetAngularVelocity(angularVelocity);
            }
            if (ImGui::DragFloat3("Linear Acceleration", &linearAcceleration.x, 0.01f)){
                //rb->AddLinear(linearAcceleration, PxForceMode::eACCELERATION);
            }
            if (ImGui::DragFloat3("Angular Acceleration", &angularAcceleration.x, 0.01f)){
                //rb->AddAngular(angularAcceleration, PxForceMode::eACCELERATION);
            }

            ImGui::PopID();
        }
    }
} // TetraEngine