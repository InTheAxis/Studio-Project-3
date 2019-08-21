#include "SpawnerScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/Spawner.h"
#include "../Scenes/ScenePlayer.h"

SpawnerScene::SpawnerScene(std::string name) : Scene(name), currentWave(0), fromPlayer(0.f, 0.f, 0.f)
{
}

SpawnerScene::~SpawnerScene()
{
}

void SpawnerScene::Start()
{
	AddChild<GameObj>("SpawnerGO")->AddComp<Sprite>();
	GetChild<GameObj>("SpawnerGO")->AddScript<Spawner>();
	SpawnerGO = GetChild<GameObj>("SpawnerGO");
	Scene::Start();
}

void SpawnerScene::Update(double dt)
{
	//int spawnRangeX = Math::RandIntMinMax(-20, 20);
	//int spawnRangeY = Math::RandIntMinMax(-4, 4);
	//SpawnerGO->GetTransform()->translate.Set(fromPlayer.x + spawnRangeX, fromPlayer.y, fromPlayer.z);
	SpawnerGO->GetTransform()->translate.Set(fromPlayer.x, 0);
	SpawnerGO->GetScript<Spawner>()->SetPlayerTrans(fromPlayer);
	SpawnerGO->GetScript<Spawner>()->SetSpawnerWave(currentWave);
	Scene::Update(dt);
}

void SpawnerScene::End()
{
	Scene::End();
}

void SpawnerScene::OnEnable()
{
}

void SpawnerScene::OnDisable()
{
}

void SpawnerScene::Render()
{
	//If got diff render pipeline

	//for (auto r : *renderables)
	//{
	//	r->Render();
	//}

	//OR

	Scene::Render();
}

void SpawnerScene::SetWave(int wave)
{
	currentWave = wave;
}

void SpawnerScene::PlayerTrans(Vector3 trans)
{
	fromPlayer = trans;
}
