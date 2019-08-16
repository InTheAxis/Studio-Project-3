#include "SpawnerScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/Spawner.h"

SpawnerScene::SpawnerScene(std::string name) : Scene(name), currentWave(0)
{
}

SpawnerScene::~SpawnerScene()
{
}

void SpawnerScene::Start()
{	
	//create gameobjects

	SplineCubic s1;
	SplineExpo s2;
	SplineLogarithm s3;
	SplineQuadratic s4;
	SplineReciprocal s5;
	SplineSqReci s6;

	AddChild<GameObj>("SpCubic")->AddComp<ColliderRender>()->AttachPoints(s1.GetSamplePts())->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("SpCubic")->GetTransform()->translate.Set(0, 0, 0);
	AddChild<GameObj>("SpExpo")->AddComp<ColliderRender>()->AttachPoints(s2.GetSamplePts())->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("SpExpo")->GetTransform()->translate.Set(1, -0.355f, 0);
	AddChild<GameObj>("SpLoga")->AddComp<ColliderRender>()->AttachPoints(s3.GetSamplePts())->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("SpLoga")->GetTransform()->translate.Set(1.4f, 2.3f, 0);
	AddChild<GameObj>("SpQuad")->AddComp<ColliderRender>()->AttachPoints(s4.GetSamplePts())->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("SpQuad")->GetTransform()->translate.Set(2.4f, 2.25f, 0);
	AddChild<GameObj>("SpReci")->AddComp<ColliderRender>()->AttachPoints(s5.GetSamplePts())->AttachMaterial(mg->GetCachedMaterial("default")); 
	GetChild<GameObj>("SpReci")->GetTransform()->translate.Set(3.4f, 0, 0);
	AddChild<GameObj>("SpSqReci")->AddComp<ColliderRender>()->AttachPoints(s6.GetSamplePts())->AttachMaterial(mg->GetCachedMaterial("default")); 
	GetChild<GameObj>("SpSqReci")->GetTransform()->translate.Set(4.4f, -3.5f, 0);
	
	AddChild<GameObj>("SpawnerGO")->GetTransform()->translate.Set(1, 2, 0);
	GetChild<GameObj>("SpawnerGO")->AddScript<Spawner>()->setWave(currentWave);

	Scene::Start();
}

void SpawnerScene::Update(double dt)
{	
	Scene::Update(dt);
} 

void SpawnerScene::End()
{
	Scene::End();
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

void SpawnerScene::setWave(int wave)
{
	currentWave = wave;
}
