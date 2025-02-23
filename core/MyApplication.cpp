#include "tetrapc.h"
#include "MyApplication.h"
#include "Core.h"
#include "rendering/Skybox.h"
#include "utils/OBJParser.h"
#include "rendering/VertexData.h"
#include "utils/Time.h"


using namespace TetraEngine;

MyApplication::MyApplication()
{
	myScene.skybox = new Skybox(Skybox::BOX, "../assets/skybox");
	Skybox::current = myScene.skybox;
	litShader = new Shader(shaderPath + "/lit.glvs", shaderPath + "/lit.glfs");
	litShader->Use();

	int shipID = OBJParser::OBJRead(meshPath + "/ship.obj");
	std::shared_ptr<VertexData> ship = VertexData::GetPrefab(shipID);
	shipRenderer = new MeshRenderer(ship, litShader);

	light1->diffuse = glm::vec3(0.1f, 1, 0.7f);
	light1->specular = light1->diffuse * 0.3f;
	light1->ambient = light1->diffuse * 0.0f;
	myScene.lightManager.AddPointLight(light1);

	light2->diffuse = glm::vec3(1, 0.7f, 0.1f);
	light2->specular = light2->diffuse * 0.3f;
	light2->ambient = light2->diffuse * 0.0f;
	myScene.lightManager.AddPointLight(light2);
	//MeshRenderer enemyRenderer = MeshRenderer(VertexData::GetPrefab(ship), &shader);
	//MeshRenderer bullet = MeshRenderer(VertexData::GetPrefab(VD_CUBE), &shader);

	ambientTex.Load(texturePath + "/ship/ambient.png", true);
	diffuseTex.Load(texturePath + "/ship/diffuse.png", true);
	specularTex.Load(texturePath + "/ship/specular.png", true);

	player = GameObject::Create(glm::vec3(0, 0, 0), "ship", shipRenderer);

	//monke1->Rotate(glm::angleAxis(45.0f, glm::vec3(10, 10, 10)));
	player->LocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
	shipRenderer->setTexture(&diffuseTex, 1);
	shipRenderer->setTexture(&specularTex, 2);
	shipRenderer->setTexture(&ambientTex, 4);

	//enemyRenderer.setTexture(&diffuseTex, 1);
	//enemyRenderer.setTexture(&specularTex, 2);
	//enemyRenderer.setTexture(&ambientTex, 4);
	myScene.AddObject(player);
	myScene.AddObject(light1);
	myScene.AddObject(light2);

	//Material emissive = Material(glm::vec3(0, 1, 1), glm::vec3(0, 1, 1), glm::vec3(0, 1, 1), "emissive");
	//Material enemyMaterial = Material(glm::vec3(0.5f, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), "ship");
	//bullet.material = &emissive;
	player->renderer->material = &shipMaterial;
}
MyApplication::~MyApplication()
{
	std::cout << "app destroyed";

	delete shipRenderer;
	delete litShader;
}
void MyApplication::Update()
{
	player->LocalRotate(glm::angleAxis(Time::sdeltaTime / 3, glm::vec3(0, 1, 0)));
}
