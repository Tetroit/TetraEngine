// #include "tetrapc.h"
// #include "Enemy.h"
// #include "../utils/Time.h"
// #include "../rendering/Scene.h"
// #include "../utils/Utils.h"
// #include "../GameObject.h"
//
// using namespace TetraEngine;
//
// Enemy::Enemy(int& score) : score(score), Behaviour("enemy")
// {
// 	lifetime = Utils::frand() + 4;
// };
//
// void Enemy::Start()
// {}
// void Enemy::Update()
// {
// 	lifetime -= Time::deltaTime;
// 	if (lifetime < 0)
// 		gameObject->Delete();
// 	gameObject->LocalTranslate(glm::vec3(30.f, 0, 0) * Time::deltaTime);
// 	for (int i = 0; i < gameObject->scene->objects.size(); i++)
// 	{
// 		GameObject* go = gameObject->scene->objects[i].get();
// 		if (go->HasScript("bullet") && CheckCollision(go))
// 		{
// 			gameObject->Delete();
// 			go->Delete();
// 			score++;
// 		}
// 	}
// }
// bool Enemy::CheckCollision(GameObject* other) {
// 	float dist = glm::length((other->GetGlobalPos() - gameObject->GetGlobalPos()));
// 	if (dist < 0.5f)
// 		return true;
// 	return false;
// }