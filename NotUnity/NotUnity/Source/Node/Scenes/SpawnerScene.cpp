#include "SpawnerScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/Spawner.h"
#include "../Scenes/ScenePlayer.h"
#include "../Manager/MgrAchievements.h"

SpawnerScene::SpawnerScene(std::string name) 
	: Scene(name)
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

	AddChild<GameObj>("WaveCounter");
	GetChild<GameObj>("WaveCounter")->GetComp<Transform>()->translate.Set(0, 5, -3);
	waveNum = GetChild<GameObj>("WaveCounter")->AddComp<Text>();
	waveNum->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("text"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("font"));
	waveNum->SetSize(0.5f);
	Scene::Start();
}

void SpawnerScene::Update(double dt)
{
	SpawnerGO->GetTransform()->translate.Set(fromPlayer.x, 0);
	SpawnerGO->GetScript<Spawner>()->SetPlayerTrans(fromPlayer);

	ss.clear(); ss.str(""); ss.precision(2);
	ss << SpawnerGO->GetScript<Spawner>()->GetSpawnerWave();
	waveNum->SetText(ss.str());
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
	SpawnerGO->GetScript<Spawner>()->SetSpawnerWave(wave);
	SpawnerGO->GetScript<Spawner>()->SetStrategy(wave);
}

int SpawnerScene::GetWave()
{
	return SpawnerGO->GetScript<Spawner>()->GetSpawnerWave();
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

void SpawnerScene::NewWave()
{
	SpawnerGO->GetScript<Spawner>()->NewWave();
}

void SpawnerScene::SetStartGame(bool start)
{
	SpawnerGO->GetScript<Spawner>()->SetStartGame(start);
}
