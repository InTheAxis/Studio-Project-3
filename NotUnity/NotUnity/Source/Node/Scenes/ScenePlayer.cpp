#include "ScenePlayer.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/Player.h"

ScenePlayer::ScenePlayer(std::string name)
	: Scene(name)
{
}

ScenePlayer::~ScenePlayer()
{
}

void ScenePlayer::Start()
{
	//create gameobjects	
	//AddChild<GameObj>("axes");
	AddChild<GameObj>("Player");
	//add & set up components and scripts	
	//GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("Player")->AddScript<Player>();

	Scene::Start();
}

void ScenePlayer::Update(double dt)
{
	Scene::Update(dt);
}

void ScenePlayer::End()
{
	Scene::End();
}

void ScenePlayer::Render()
{
	//If got diff render pipeline

	//for (auto r : *renderables)
	//{
	//	r->Render();
	//}

	//OR

	Scene::Render();
}