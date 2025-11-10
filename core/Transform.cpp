//
// Created by pchyo on 30/09/2025.
//

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif


#include "Transform.h"
#include "Core.h"
#include "GameObject.h"
#include <execution>
#include <stack>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "rendering/Scene.h"


void TetraEngine::Transform::ComponentCreate(Transform &transform, ECS::Entity entity, ECS::Handle<Transform> handle) {
    transform.self = handle;
}

TetraEngine::Transform::Transform() :
isDirty(false),
localMatrix(glm::mat4(1.0f)),
globalMatrix(glm::mat4(1.0f)),
parent (ECS::Handle<Transform>::CreateInvalid()),
children (std::vector<ECS::Handle<Transform> >()) {}


void TetraEngine::Transform::MarkDirty() {
    isDirty = true;
}

bool TetraEngine::Transform::IsDirty() const {
    return isDirty;
}

void TetraEngine::Transform::SetParent(ECS::Handle<Transform> transform) {

    auto newParentPtr = Core::GetMainECS().GetComponent(transform);
    if (newParentPtr == nullptr) {
        LOG_ERR("parent transform was not found");
        return;
    }
    if (auto parentPtr = GetParent(); parentPtr != nullptr) {
        auto parentLoc = std::ranges::find_if(parentPtr->children,
            [this](ECS::Handle<Transform> tr) {
            return Core::GetMainECS().GetComponent(tr) == this;
        });
        if (parentLoc != parentPtr->children.end()) {
            parentPtr->children.erase(parentLoc);
        }
    }
    parent = transform;
    //newParentPtr->children.push_back(???); //bruh I cannot get this yet ;-;
}

void TetraEngine::Transform::SetParent(ECS::Handle<Transform> transform, ECS::Handle<Transform> parent) {

    auto newParentPtr = Core::GetMainECS().GetComponent(parent);
    auto current = Core::GetMainECS().GetComponent(transform);
    if (current == nullptr) {
        LOG_ERR("current transform was not found");
        return;
    }
    auto infoCurrent = Core::GetMainECS().GetRelatedComponent<GameObjectInfo>(transform);
    auto infoParent = Core::GetMainECS().GetRelatedComponent<GameObjectInfo>(parent);
    if (infoCurrent->scene != infoParent->scene) {
        if (infoCurrent->scene) {
            infoCurrent->scene->RemoveObject(infoCurrent->entity);
        }
        if (infoParent->scene) {
            infoParent->scene->AddObject(infoCurrent->entity);
        }
    }

    if (auto parentPtr = current->GetParent(); parentPtr != nullptr) {
        auto parentLoc = std::ranges::find(parentPtr->children, transform);
        if (parentLoc != parentPtr->children.end()) {
            parentPtr->children.erase(parentLoc);
        }
    }
    current->parent = parent;
    if (newParentPtr != nullptr)
        newParentPtr->children.push_back(transform);
    current->OnSetParent.Call(parent, transform);
    current->MarkDirty();
}


TetraEngine::Transform * TetraEngine::Transform::GetParent() const {
    return Core::GetMainECS().GetComponent(parent);
}
TetraEngine::ECS::Handle<TetraEngine::Transform> TetraEngine::Transform::GetParentHandle() const {
    return parent;
}

TetraEngine::Transform * TetraEngine::Transform::GetChild(uint id) const {
    return Core::GetMainECS().GetComponent(children[id]);
}

std::ranges::subrange<std::vector<TetraEngine::ECS::Handle<TetraEngine::Transform>>::iterator> TetraEngine::Transform::GetChildren() {

    return std::ranges::subrange(children.begin(), children.end());
}
std::ranges::subrange<std::vector<TetraEngine::ECS::Handle<TetraEngine::Transform>>::const_iterator> TetraEngine::Transform::GetChildrenConst() const {

    return std::ranges::subrange(children.cbegin(), children.cend());
}

glm::mat4 TetraEngine::Transform::GetLocalMatrix() const {

    return  localMatrix;
}

glm::mat4 TetraEngine::Transform::GetGlobalMatrix() {
    if (IsDirty()) {
        Recalculate();
    }
    return  globalMatrix;
}

glm::mat4 TetraEngine::Transform::GetParentMatrix() {
    if (!parent.Valid()) {
        return glm::identity<glm::mat4>();
    }
    return GetParent()->GetGlobalMatrix();
}

void TetraEngine::Transform::SetGlobalMatrix(glm::mat4&& newMatrix) {
    localMatrix = glm::inverse(GetParentMatrix()) * newMatrix;
    MarkDirty();
}

void TetraEngine::Transform::Recalculate() {
    auto parentPtr = GetParent();
    auto parentMat = glm::mat4(1.0);
    if (parentPtr) {
        parentMat = parentPtr->globalMatrix;
    }
    globalMatrix = parentMat * localMatrix;
    for (auto child : children) {
        Core::GetMainECS().GetComponent(child)->RecalculateWith(globalMatrix);
    }
    isDirty = false;
}

void TetraEngine::Transform::RecalculateGlobalMatrix() {

    auto par = GetParent();
    Transform* lastDirty = nullptr;
    while (par != nullptr)
    {
        if (par->IsDirty())
            lastDirty = par;
        par = par->GetParent();
    }
    if (lastDirty)
        lastDirty->Recalculate();
}

void TetraEngine::Transform::RecalculateWith(glm::mat4 m) {
    globalMatrix = m * localMatrix;
    for (auto child : children) {
        Core::GetMainECS().GetComponent(child)->RecalculateWith(globalMatrix);
    }
    isDirty = false;
}

void TetraEngine::Transform::LocalTranslate(glm::vec3 pos)
{
    localMatrix = glm::translate(localMatrix, pos);
    MarkDirty();
}
void TetraEngine::Transform::LocalRotate(glm::quat rot)
{
    localMatrix *= glm::toMat4(glm::normalize(rot));
    MarkDirty();
}
void TetraEngine::Transform::LocalScale(glm::vec3 scale)
{
    localMatrix = glm::scale(localMatrix, scale);
    MarkDirty();
}

void TetraEngine::Transform::GlobalTranslate(glm::vec3 pos)
{
    localMatrix = glm::translate(localMatrix, glm::vec3(glm::inverse(globalMatrix) * glm::vec4(pos, 0)));
    MarkDirty();
}
void TetraEngine::Transform::GlobalRotate(glm::quat rot)
{
    localMatrix *= glm::inverse(globalMatrix) * glm::toMat4(glm::normalize(rot)) * globalMatrix;
    MarkDirty();
}
void TetraEngine::Transform::GlobalScale(glm::vec3 scale)
{
    localMatrix *= glm::inverse(globalMatrix) * glm::scale(scale) * globalMatrix;
    MarkDirty();
}

glm::vec3 TetraEngine::Transform::GetPosition() {
    if (isDirty) Recalculate();
    return {globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2]};
}

glm::quat TetraEngine::Transform::GetRotation() {
    if (isDirty) Recalculate();
    return glm::normalize(glm::toQuat(globalMatrix));
}

glm::vec3 TetraEngine::Transform::GetScale() {
    if (isDirty) Recalculate();
    return {
        glm::length(glm::vec3(globalMatrix[0])),
        glm::length(glm::vec3(globalMatrix[1])),
        glm::length(glm::vec3(globalMatrix[2]))
    };
}

glm::vec3 TetraEngine::Transform::GetLocalPosition() {

    return {localMatrix[3][0], localMatrix[3][1], localMatrix[3][2]};
}
glm::quat TetraEngine::Transform::GetLocalRotation() {
    return glm::normalize(glm::toQuat(localMatrix));
}

glm::vec3 TetraEngine::Transform::GetLocalScale() {
    return {
        glm::length(glm::vec3(localMatrix[0])),
        glm::length(glm::vec3(localMatrix[1])),
        glm::length(glm::vec3(localMatrix[2]))
    };
}

void TetraEngine::Transform::SetPosition(glm::vec3 pos) {

    SetLocalPosition( glm::inverse(GetParentMatrix()) * glm::vec4(pos, 1) );
    MarkDirty();
}

void TetraEngine::Transform::SetRotation(glm::quat rot) {
    auto currentRot = GetRotation();
    GlobalRotate(rot);
    MarkDirty();
    //WORK IN PROGRESS
}

void TetraEngine::Transform::SetScale(glm::vec3 scale) {

    //WORK IN PROGRESS
}

void TetraEngine::Transform::SetLocalPosition(glm::vec3 pos) {

    localMatrix[3][0] = pos.x;
    localMatrix[3][1] = pos.y;
    localMatrix[3][2] = pos.z;
    MarkDirty();
}

void TetraEngine::Transform::SetLocalRotation(glm::quat rot) {

    //WORK IN PROGRESS
}

void TetraEngine::Transform::SetLocalScale(glm::vec3 scale) {
    for (int i = 0; i < 3; i++) {
        int len = glm::length(glm::vec3(localMatrix[i]));
        for (int j = 0; j < 3; j++) {
            localMatrix[i][j] = localMatrix[j][i] / len * scale[i];
        }
    }
    MarkDirty();
}

