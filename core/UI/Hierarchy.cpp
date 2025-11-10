//
// Created by pchyo on 06/10/2025.
//

#include "Hierarchy.h"

#include "imgui_internal.h"
#include "../Core.h"
#include "../GameObject.h"
#include "../Transform.h"

using namespace TetraEngine;


UI::GameObjectTreeEntry::GameObjectTreeEntry(const Transform &tr, const GameObjectInfo &info):
    selfID(info.entity),
    name(info.name.c_str()),
    parentID(ECS::Entity()),
    idInParent(-1),
    expanded(false) {}

UI::GameObjectTreeEntry::~GameObjectTreeEntry() {
}


void UI::Hierarchy::ParseInfo() {
    roots.clear();
    Core::GetMainECS().Foreach<Transform, GameObjectInfo>([&](const Transform& tr, const GameObjectInfo& info) {
        if (tr.GetParent() == nullptr) {
            roots.push_back(info.entity);
            BuildTreeFrom(tr, info);
        }
    });
}

ECS::Entity UI::Hierarchy::GetSelected() {
    return selected;
}


void UI::Hierarchy::ParentNodes(ECS::Entity parent, ECS::Entity child) {
    auto& childNode = nodes.at(child);
    auto& parentNode = nodes.at(parent);
    auto exParentNodeID = childNode.parentID;

    if (exParentNodeID.isValid()) {

        auto& exParentNode = nodes.at(childNode.parentID);
        auto swapChildID = exParentNode.children[exParentNode.children.size() - 1];
        auto& swapChild = nodes.at(swapChildID);
        exParentNode.children[childNode.idInParent] = swapChildID;
        swapChild.idInParent = childNode.idInParent;
        exParentNode.children.pop_back();
    }
    if (parent.isValid()) {

        parentNode.children.push_back(child);
        childNode.parentID = parent;
        childNode.idInParent = parentNode.children.size() - 1;
    }
    else {
        childNode.idInParent = -1;
    }
}

Action<ECS::Entity> & UI::Hierarchy::OnSelected() {
    return onSelectAction;
}

void UI::Hierarchy::Draw(bool* isDrawn) {
    if (ImGui::Begin("Hierarchy", isDrawn)) {
        ParseInfo();
        for (const auto& nodeID : roots) {
            Draw(nodes.at(nodeID));
        }
    }
    ImGui::End();
}

void UI::Hierarchy::Draw(const GameObjectTreeEntry &node) {

    ImGui::PushID(node.selfID.ID);

    [[maybe_unused]]
    const auto& stack = ImGui::GetCurrentWindow()->IDStack;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

    flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    flags |= ImGuiTreeNodeFlags_NavLeftJumpsBackHere;

    if (selected == node.selfID) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (node.children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    bool isOpen = ImGui::TreeNodeEx("", flags, "%s", node.name);
    if (ImGui::IsItemFocused()) {

        selected = node.selfID;
        onSelectAction.Call(node.selfID);
    }
    if (isOpen) {
        for (const auto& child : node.children) {
            Draw(nodes.at(child));
        }
        ImGui::TreePop();
    }

    HierarchyIDStack = ImGui::GetCurrentWindow()->IDStack.size();


    ImGui::PopID();
    HierarchyIDStack--;
    //THIS IS CRAP
}

void UI::Hierarchy::BindEvents() {
    TETRA_USE_MAIN_ECS
    ecs.OnComponentDestroyed<Transform>().AddCallback(
        [&](TETRA_COMPONENT_EVENT_LISTENER_PARAMS(Transform)) {
        OnDestroy(component, entity, handle);
    }, "Hierarchy");
}

void UI::Hierarchy::OnDestroy (TETRA_COMPONENT_EVENT_LISTENER_PARAMS(Transform)) {
    nodes.erase(nodes.find(entity));
}

UI::GameObjectTreeEntry* UI::Hierarchy::BuildTreeFrom(const Transform& tr, const GameObjectInfo& info) {
    TETRA_USE_MAIN_ECS
    auto[it, success] = nodes.try_emplace(info.entity, GameObjectTreeEntry(tr, info));
    auto& node = it->second;
    for (auto& child : tr.GetChildrenConst()) {
        auto childInfo = ecs.GetRelatedComponent<GameObjectInfo, Transform>(child);
        auto childTr = ecs.GetRelatedComponent<Transform>(child);
        if (!childTr || !childInfo) {
            continue;
        }
        auto generated = BuildTreeFrom(*childTr, *childInfo);
        if (generated) {
            ParentNodes(node.selfID, generated->selfID);
        }
    }
    return &node;
}
