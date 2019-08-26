#include "SpawnerScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/Spawner.h"
#include "../Scenes/ScenePlayer.h"
#include "../Manager/MgrAchievements.h"

SpawnerScene::SpawnerScene(std::string name) 
	: Scene(name)
	, currentWave(0)
	, fromPlayer(0.f, 0.f, 0.f)
	, SpawnerGO(nullptr)
{
}

SpawnerScene::~SpawnerScene()
{
	SpawnerGO = nullptr;
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

void SpawnerScene::SetTerrain(Spline* s)
{
	SpawnerGO->GetScript<Spawner>()->SetTerrain(s);
}

void SpawnerScene::Reset()
{
	SpawnerGO->GetScript<Spawner>()->Reset();
}

int SpawnerScene::GetEnemyKilled()
{

	return SpawnerGO->GetScript<Spawner>()->GetEnemiesKilled();
}

int SpawnerScene::GetSpawnerWave()
{
	return SpawnerGO->GetScript<Spawner>()->GetSpawnerWave();
}

bool SpawnerScene::GetBossKilled()
{
	return SpawnerGO->GetScript<Spawner>()->GetBossKilled();
}

void SpawnerScene::NewWave(int wave)
{
	SpawnerGO->GetScript<Spawner>()->NewWave();
	SpawnerGO->GetScript<Spawner>()->SetStrategy(wave);
}