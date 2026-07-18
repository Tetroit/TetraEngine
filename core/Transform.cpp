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

using namespace TetraEngine;

void Transform::ComponentCreate(Transform &transform, ECS::Entity entity, ECS::Handle<Transform> handle) {
    transform.self = handle;
}

Transform::Transform() :
isDirty(false),
position(glm::vec3(0.0f)),
rotation(glm::quat(1.0f,0.0f,0.0f,0.0f)),
scale(glm::vec3(1.0f)),

g_position(glm::vec3(0.0f)),
g_rotation(glm::quat(1.0f,0.0f,0.0f,0.0f)),
g_scale(glm::vec3(1.0f)),

localMatrix(glm::mat4(1.0f)),
globalMatrix(glm::mat4(1.0f)),

parent (ECS::Handle<Transform>::CreateInvalid()),
children (std::vector<ECS::Handle<Transform> >()) {}


void Transform::MarkDirty() {
    if (!isDirty) {
        isDirty = true;
        for (int i = 0; i<children.size(); i++) {
            GetChild(i)->MarkDirty();
        }
    }
}

bool Transform::IsDirty() const {
    return isDirty;
}

void Transform::SetParent(ECS::Handle<Transform> transform) {

    auto newParentPtr = Core::GetMainECS()->GetComponent(transform);
    if (newParentPtr == nullptr) {
        LOG_ERR("parent transform was not found");
        return;
    }
    if (auto parentPtr = GetParent(); parentPtr != nullptr) {
        auto parentLoc = std::ranges::find_if(parentPtr->children,
            [this](ECS::Handle<Transform> tr) {
            return Core::GetMainECS()->GetComponent(tr) == this;
        });
        if (parentLoc != parentPtr->children.end()) {
            parentPtr->children.erase(parentLoc);
        }
    }
    parent = transform;
    //newParentPtr->children.push_back(???); //bruh I cannot get this yet ;-;
}

void Transform::SetParent(ECS::Handle<Transform> transform, ECS::Handle<Transform> parent) {

    auto newParentPtr = Core::GetMainECS()->GetComponent(parent);
    auto current = Core::GetMainECS()->GetComponent(transform);
    if (current == nullptr) {
        LOG_ERR("current transform was not found");
        return;
    }
    auto infoCurrent = Core::GetMainECS()->GetRelatedComponent<GameObjectInfo>(transform);
    auto infoParent = Core::GetMainECS()->GetRelatedComponent<GameObjectInfo>(parent);
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


Transform * Transform::GetParent() const {
    return Core::GetMainECS()->GetComponent(parent);
}

void Transform::ClearParent() {
    if (auto parentPtr = GetParent(); parentPtr != nullptr) {
        auto parentLoc = std::ranges::find(parentPtr->children, self);
        if (parentLoc != parentPtr->children.end()) {
            parentPtr->children.erase(parentLoc);
        }
    }
    parent = ECS::Handle<Transform>::CreateInvalid();
}

ECS::Handle<Transform> Transform::GetParentHandle() const {
    return parent;
}

Transform* Transform::GetChild(uint id) const {
    return Core::GetMainECS()->GetComponent(children[id]);
}

std::ranges::subrange<std::vector<ECS::Handle<Transform>>::iterator> Transform::GetChildren() {

    return {children.begin(), children.end()};
}
std::ranges::subrange<std::vector<ECS::Handle<Transform>>::const_iterator> Transform::GetChildrenConst() const {

    return {children.cbegin(), children.cend()};
}

glm::mat4 Transform::GetLocalMatrix() {
    if (IsDirty()) {
        Recalculate();
    }
    return  localMatrix;
}

glm::mat4 Transform::GetGlobalMatrix() {
    if (IsDirty()) {
        Recalculate();
    }
    return  globalMatrix;
}

glm::mat4 Transform::GetParentMatrix() {
    if (!parent.Valid()) {
        return glm::identity<glm::mat4>();
    }
    return GetParent()->GetGlobalMatrix();
}

void Transform::Recalculate() {

    localMatrix = glm::translate(glm::mat4(1.0f), position)
                    * glm::toMat4(rotation)
                    * glm::scale(glm::mat4(1.0f), scale);

    auto parentPtr = GetParent();

    if (parentPtr) {
        if (parentPtr->IsDirty())
            parentPtr->Recalculate();

        g_rotation = parentPtr->g_rotation * rotation;
        g_scale = parentPtr->g_scale * scale;
        g_position = parentPtr->g_position + parentPtr->g_rotation * (parentPtr->g_scale * position);
        globalMatrix = parentPtr->globalMatrix * localMatrix;
    }
    else {
        g_rotation = rotation;
        g_scale = scale;
        g_position = position;
        globalMatrix = localMatrix;
    }

    isDirty = false;
}

void Transform::LocalTranslate(glm::vec3 pos)
{
    position += pos;
    MarkDirty();
}
void Transform::LocalRotate(glm::quat rot)
{
    rotation *= rot;
    MarkDirty();
}
void Transform::LocalScale(glm::vec3 sc)
{
    scale *= sc;
    MarkDirty();
}
void Transform::LocalScale(float sc)
{
    scale *= sc;
    MarkDirty();
}

void Transform::GlobalTranslate(glm::vec3 pos) {
    if (parent.Valid()) {
        position += glm::inverse(GetParentMatrix()) * glm::vec4(pos, 1);
    }
    else {
        position += pos;
    }
    MarkDirty();
}

void Transform::GlobalRotate(glm::quat rot)
{
    if (parent.Valid()) {
        glm::quat parentRot = GetParent()->GetRotation();
        rotation = glm::inverse(parentRot) * rot * parentRot * rotation;
    }
    else {
        rotation *= rot;
    }
    MarkDirty();
}
void Transform::GlobalScale(glm::vec3 sc)
{
    if (parent.Valid()) {
        auto parentMat = GetParent()->GetGlobalMatrix();
        glm::vec3 parentScale(1);
        parentScale.x = glm::length(glm::vec3(parentMat[0]));
        parentScale.y = glm::length(glm::vec3(parentMat[1]));
        parentScale.z = glm::length(glm::vec3(parentMat[2]));
        scale *= parentScale * sc;
    }
    else {
        scale *= sc;
    }
}

glm::vec3 Transform::GetPosition() {
    if (isDirty) Recalculate();
    return g_position;
}

glm::quat Transform::GetRotation() {
    if (isDirty) Recalculate();
    return g_rotation;
}

glm::vec3 Transform::GetScale() {
    if (isDirty) Recalculate();
    return g_scale;
}

glm::vec3 Transform::GetLocalPosition() {
    return position;
}
glm::quat Transform::GetLocalRotation() {
    return rotation;
}
glm::vec3 Transform::GetLocalScale() {
    return scale;
}

void Transform::SetPosition(glm::vec3 pos) {

    if (parent.Valid()) {
        position = glm::inverse(GetParentMatrix()) * glm::vec4(pos, 1);
    }
    else {
        position = pos;
    }
    MarkDirty();
}

void Transform::SetRotation(glm::quat rot) {
    if (parent.Valid()) {
        rotation = glm::inverse(GetParent()->GetRotation()) * rot;
    }
    else {
        rotation = rot;
    }
}

void Transform::SetScale(glm::vec3 sc) {

    if (parent.Valid()) {
        auto parentMat = GetParent()->GetGlobalMatrix();
        glm::vec3 parentScale(1);
        parentScale.x = glm::length(glm::vec3(parentMat[0]));
        parentScale.y = glm::length(glm::vec3(parentMat[1]));
        parentScale.z = glm::length(glm::vec3(parentMat[2]));
        scale = parentScale * sc;
    }
    else {
        scale = sc;
    }
}

void Transform::SetLocalPosition(glm::vec3 pos) {

    position = pos;
    MarkDirty();
}

void Transform::SetLocalRotation(glm::quat rot) {

    rotation = rot;
    MarkDirty();
}

void Transform::SetLocalScale(glm::vec3 sc) {
    scale = sc;
    MarkDirty();
}

glm::vec3 Transform::TransformDirectionToWorld(glm::vec3 pos) {
    if (isDirty) Recalculate();
    glm::vec4 vec = glm::vec4(pos, 0);
    return globalMatrix * vec;
}

glm::vec3 Transform::TransformDirectionToLocal(glm::vec3 pos) {
    if (isDirty) Recalculate();
    glm::vec4 vec = glm::vec4(pos, 0);
    return glm::inverse(globalMatrix) * vec;
}
glm::vec3 Transform::TransformPointToWorld(glm::vec3 pos) {
    if (isDirty) Recalculate();
    glm::vec4 vec = glm::vec4(pos, 1);
    return globalMatrix * vec;
}

glm::vec3 Transform::TransformPointToLocal(glm::vec3 pos) {
    if (isDirty) Recalculate();
    glm::vec4 vec = glm::vec4(pos, 1);
    return glm::inverse(globalMatrix) * vec;
}

