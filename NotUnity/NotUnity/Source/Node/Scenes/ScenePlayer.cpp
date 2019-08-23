#include "ScenePlayer.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/Player.h"
#include "../Scripts/PlayerController.h"
#include "../Scripts/ColorSpot.h"

ScenePlayer::ScenePlayer(std::string name)
	: Scene(name)
{
}

ScenePlayer::~ScenePlayer()
{
	colorSpot = nullptr;
	playerScript = nullptr;
}

void ScenePlayer::Start()
{
	//create gameobjects	
	AddChild<GameObj>("Player");
	
	//add & set up components and scripts	
	playerScript = GetChild<GameObj>("Player")->AddScript<PlayerController>();

	colorSpot = GetChild<GameObj>("Player")->AddComp<ColorSpot>();

	Scene::Start();
}

void ScenePlayer::Update(double dt)
{
	colorSpot->SetUniform(0);

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

void ScenePlayer::SetTerrain(Spline* s)
{
	playerScript->SetTerrain(s);
}
