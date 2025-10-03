// #include "tetrapc.h"
// #include "GameObject.h"
//
// #ifndef GLM_ENABLE_EXPERIMENTAL
// #define GLM_ENABLE_EXPERIMENTAL
// #endif
//
// #include <glm/gtx/quaternion.hpp>
//
// #include <glm/gtx/transform.hpp>
//
// #include "rendering/Scene.h"
// #include "Behaviour.h"
//
// using namespace TetraEngine;
//
// glm::mat4 GameObject::currentTransform = glm::mat4(1);
//
// GameObject::GameObject(glm::vec3 pos, const std::string name, MeshRenderer* meshRenderer) : renderer(meshRenderer)
// {
// 	this->name = name;
// 	parent = nullptr;
// 	scene = nullptr;
// 	transform = glm::mat4(1);
// 	localToGlobal = glm::mat4(1);
// 	renderer = meshRenderer;
// 	transform[3][0] = pos.x;
// 	transform[3][1] = pos.y;
// 	transform[3][2] = pos.z;
// 	for (int i = 0; i < scripts.size(); i++)
// 	{
// 		scripts[i]->Start();
// 	}
// }
// GameObject::~GameObject() {
// 	for (Behaviour* script : scripts)
// 	{
// 		if (script->destroyWithObject)
// 			delete script;
// 	}
// 	std::cout << name << " destroyed\n";
// }
// void GameObject::AddChild (GameObject* child)
// {
// 	children.push_back(child);
// 	child->parent = this;
// 	scene->AddObject(child);
// }
//
// GameObject* GameObject::Create(glm::vec3 pos, const std::string name, MeshRenderer* meshRenderer) {
// 	GameObject* ptr = new GameObject(pos, name, meshRenderer);
// 	return ptr;
// }
// void GameObject::AddBehaviour(Behaviour* script) {
// 	scripts.push_back(script);
// 	script->gameObject = this;
// }
// void GameObject::UpdateMatrix() {
//
// 	GameObject* par = parent;
// 	glm::mat4 global = transform;
// 	while (par != nullptr)
// 	{
// 		global *= par->transform;
// 		par = par->parent;
// 	}
// 	localToGlobal = global;
// }
// void GameObject::Render()
// {
// 	currentTransform *= transform;
// 	renderer->Render(currentTransform);
// 	localToGlobal = currentTransform;
// 	for (GameObject* go : children) {
// 		go->Render();
// 	}
// 	currentTransform *= glm::inverse(transform);
// }
// void GameObject::Update() {
// 	for (int i = 0; i < scripts.size(); i++)
// 	{
// 		scripts[i]->Update();
// 	}
// 	for (int i = 0; i < children.size(); i++)
// 	{
// 		children[i]->Update();
// 	}
// }
// void GameObject::OnSceneAdded(Scene* scene)
// {
// 	std::cout << "Object added to scene\n";
// }
// void GameObject::Delete()
// {
// 	if (scene != nullptr)
// 		scene->RemoveObject(this);
// }
// void GameObject::OnSceneRemoved()
// {
// 	std::cout << "Object removed from scene\n";
// }
// void GameObject::SetPosition(glm::vec3 pos)
// {
// 	glm::vec3 localPos;
// 	if (parent == nullptr)
// 		localPos = pos;
// 	else
// 		localPos = glm::vec3(glm::inverse(localToGlobal) * transform * glm::vec4(pos, 0));
//
// 	transform[3][0] = localPos.x;
// 	transform[3][1] = localPos.y;
// 	transform[3][2] = localPos.z;
// }
// void GameObject::SetRotation(glm::quat rot)
// {
// 	glm::vec3 pos = GetPos();
// 	glm::vec3 scale = GetScale();
// 	glm::mat4 newTransform = glm::toMat4(rot);
// 	newTransform = glm::translate(pos) * newTransform * glm::scale(scale);
// 	transform = newTransform;
// }
// glm::vec3 GameObject::TransformPoint(glm::vec3 point)
// {
// 	return transform * glm::vec4(point, 1);
// }
// glm::vec3 GameObject::TransformDirection(glm::vec3 point)
// {
// 	return transform * glm::vec4(point, 0);
// }
//
//
//
// void GameObject::LocalScale(glm::vec3 scale)
// {
// 	transform = glm::scale(transform, scale);
// }
// void GameObject::LocalRotate(glm::quat rot)
// {
// 	transform *= glm::toMat4(glm::normalize(rot));
// }
// void GameObject::LocalTranslate(glm::vec3 pos)
// {
// 	transform = glm::translate(transform, pos);
// }
// void GameObject::GlobalScale(glm::vec3 scale)
// {
// 	transform *= glm::inverse(localToGlobal) * glm::scale(scale) * localToGlobal;
// }
// void GameObject::GlobalRotate(glm::quat rot)
// {
// 	transform *= glm::inverse(localToGlobal) * glm::toMat4(glm::normalize(rot)) * localToGlobal;
// }
// void GameObject::GlobalTranslate(glm::vec3 pos)
// {
// 	transform = glm::translate(transform, glm::vec3(glm::inverse(localToGlobal) * glm::vec4(pos, 0)));
// }
//
//
// glm::vec3 GameObject::GetGlobalPos()
// {
// 	return glm::vec3(localToGlobal[3][0], localToGlobal[3][1], localToGlobal[3][2]);
// }
//
// glm::vec3 GameObject::GetPos() {
// 	return glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
// }
// glm::quat GameObject::GetRotation() {
// 	return glm::normalize (glm::toQuat(transform));
// }
// glm::quat GameObject::GetGlobalRotation() {
// 	return glm::normalize(glm::toQuat(localToGlobal));
// }
// glm::vec3 GameObject::GetScale() {
// 	return glm::vec3(
// 		glm::length(glm::vec3(transform[0])),
// 		glm::length(glm::vec3(transform[1])),
// 		glm::length(glm::vec3(transform[2]))
// 	);
// }
// glm::vec3 GameObject::GetGlobalScale() {
// 	return glm::vec3(
// 		glm::length(glm::vec3(localToGlobal[0])),
// 		glm::length(glm::vec3(localToGlobal[1])),
// 		glm::length(glm::vec3(localToGlobal[2]))
// 	);
// }
// bool GameObject::HasScript(std::string tag)
// {
// 	for (int i = 0; i < scripts.size(); i++)
// 		if (tag == scripts[i]->tag)
// 			return true;
// 	return false;
// }