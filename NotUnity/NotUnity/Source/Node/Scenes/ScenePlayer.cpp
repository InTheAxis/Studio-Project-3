#include "ScenePlayer.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/Player.h"
#include "../Scripts/PlayerController.h"

ScenePlayer::ScenePlayer(std::string name)
	: Scene(name)
{
}

ScenePlayer::~ScenePlayer()
{
	playerScript = nullptr;
}

void ScenePlayer::Start()
{
	//create gameobjects	
	AddChild<GameObj>("Player");
	
	//add & set up components and scripts	
	playerScript = GetChild<GameObj>("Player")->AddScript<PlayerController>();

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
	Scene::Render();
}

int ScenePlayer::GetHealth()
{
	return playerScript->GetHealth();
}

GameObj* ScenePlayer::GetPlayer()
{
	return playerScript->GetGameObj();
}

ScenePlayer* ScenePlayer::SetTerrain(Spline* s)
{
	playerScript->SetTerrain(s);
	return this;
}

ScenePlayer* ScenePlayer::SetColorSpotRad(float radius)
{
	playerScript->SetColorSpotRad(radius);
	return this;
}
