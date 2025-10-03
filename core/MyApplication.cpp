#include "tetrapc.h"
#include "MyApplication.h"
#include "Core.h"
#include "GameObject.h"
#include "rendering/Skybox.h"
#include "utils/OBJParser.h"
#include "rendering/VertexData.h"
#include "utils/Time.h"


using namespace TetraEngine;

MyApplication::MyApplication()
{
	myScene.skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
	Skybox::current = myScene.skybox;
	litShader = new Shader(shaderPath + "/lit.glvs", shaderPath + "/lit.glfs");
	litShader->Use();

	shipObject = new GameObject();
	int shipID = OBJParser::OBJRead(meshPath + "/ship.obj");
	std::shared_ptr<VertexData> ship = VertexData::GetPrefab(shipID);
	shipRenderer = shipObject->AddComponent<MeshRenderer>(ship, litShader);
	auto shipRendererPtr = Core::GetMainECS().GetComponent(shipRenderer);

	ambientTex.Load(assetPath + "/ship/ambient.png", true);
	diffuseTex.Load(assetPath + "/ship/diffuse.png", true);
	specularTex.Load(assetPath + "/ship/specular.png", true);

	shipRendererPtr->setTexture(&diffuseTex, 1);
	shipRendererPtr->setTexture(&specularTex, 2);
	shipRendererPtr->setTexture(&ambientTex, 4);

	shipRendererPtr->material = &shipMaterial;

	Scene::currentScene->AddObject(shipObject);



	cube1 = new GameObject();
	cube2 = new GameObject();
	cube3 = new GameObject();
	auto tr1 = cube1->GetTransformHandle();
	auto tr2 = cube2->GetTransformHandle();
	auto tr3 = cube3->GetTransformHandle();
	auto shipTr = shipObject->GetTransformHandle();

	Transform::SetParent(tr1, shipTr);
	Transform::SetParent(tr2, tr1);
	Transform::SetParent(tr3, tr2);
	Transform::SetParent(tr3, tr1);

	auto mr1 = cube1->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), litShader);
	auto mr2 = cube2->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), litShader);
	auto mr3 = cube3->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), litShader);

	Core::GetMainECS().GetComponent(mr3)->material = new Material(glm::vec3(0.1,0,0.1),glm::vec3(0.5,0,0.5),glm::vec3(0.3,0,0.3));
	Core::GetMainECS().GetComponent(mr2)->material = new Material(glm::vec3(0,0.1,0.1),glm::vec3(0,0.5,0.5),glm::vec3(0,0.3,0.3));
	Core::GetMainECS().GetComponent(mr1)->material = new Material(glm::vec3(0,0,0.1),glm::vec3(0,0,0.5),glm::vec3(0,0,0.3));

	auto tr1ptr = Core::GetMainECS().GetComponent(tr1);
	tr1ptr->LocalTranslate(glm::vec3(5, 0, 0));
	auto tr2ptr = Core::GetMainECS().GetComponent(tr2);
	tr2ptr->LocalTranslate(glm::vec3(0, 5, 0));
	auto tr3ptr = Core::GetMainECS().GetComponent(tr3);
	tr3ptr->LocalTranslate(glm::vec3(0, 0, 5));

	Scene::currentScene->AddObject(cube1);
	Scene::currentScene->AddObject(cube2);
	Scene::currentScene->AddObject(cube3);

	light = new GameObject();
	glm::vec3 lightCol = glm::vec3(1, 0, 1);
	auto lightBbHandle = light->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_RECTANGLE), Shader::billboardShader);
	auto lightBbPtr = Core::GetMainECS().GetComponent(lightBbHandle);
	lightBbPtr->material = new Material(lightCol, glm::vec3(0), glm::vec3(0));

	light->GetTransform()->GlobalTranslate(glm::vec3(0,0,-5));
	auto plHandle = light->AddComponent<PointLight>(lightCol * 0.1f, lightCol * 0.7f, lightCol * 0.6f, glm::vec3(1.0f, 0.1f, 0.01f));
	Scene::currentScene->AddObject(light);

	light2 = new GameObject();
	glm::vec3 light2Col = glm::vec3(0, 1, 0);
	auto light2BbHandle = light2->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_RECTANGLE), Shader::billboardShader);
	auto light2BbPtr = Core::GetMainECS().GetComponent(light2BbHandle);
	light2BbPtr->material = new Material(light2Col, glm::vec3(0), glm::vec3(0));

	light2->GetTransform()->GlobalTranslate(glm::vec3(0,0,5));
	auto plHandle2 = light2->AddComponent<PointLight>(light2Col * 0.1f, light2Col * 0.7f, light2Col * 0.6f, glm::vec3(1.0f, 0.1f, 0.01f));
	Scene::currentScene->AddObject(light2);

	// light1->diffuse = glm::vec3(0.1f, 1, 0.7f);
	// light1->specular = light1->diffuse * 0.3f;
	// light1->ambient = light1->diffuse * 0.0f;
	//myScene.lightManager.AddPointLight(light1);

	// light2->diffuse = glm::vec3(1, 0.7f, 0.1f);
	// light2->specular = light2->diffuse * 0.3f;
	// light2->ambient = light2->diffuse * 0.0f;
	//myScene.lightManager.AddPointLight(light2);
	//MeshRenderer enemyRenderer = MeshRenderer(VertexData::GetPrefab(ship), &shader);
	//MeshRenderer bullet = MeshRenderer(VertexData::GetPrefab(VD_CUBE), &shader);

	//enemyRenderer.setTexture(&diffuseTex, 1);
	//enemyRenderer.setTexture(&specularTex, 2);
	//enemyRenderer.setTexture(&ambientTex, 4);
	//myScene.AddObject(light1);
	//myScene.AddObject(light2);

	//Material emissive = Material(glm::vec3(0, 1, 1), glm::vec3(0, 1, 1), glm::vec3(0, 1, 1), "emissive");
	//Material enemyMaterial = Material(glm::vec3(0.5f, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), "ship");
	//bullet.material = &emissive;
}
MyApplication::~MyApplication()
{
	std::cout << "app destroyed";

	delete shipObject;
	delete litShader;
}
void MyApplication::Update()
{
	shipObject->GetComponent<Transform>()->LocalRotate(glm::angleAxis(Time::sdeltaTime / 3, glm::vec3(0, 1, 0)));
}
