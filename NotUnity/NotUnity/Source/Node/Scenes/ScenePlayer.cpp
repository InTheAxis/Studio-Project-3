#include "ScenePlayer.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/Player.h"
#include "../Scripts/ColorSpot.h"

ScenePlayer::ScenePlayer(std::string name)
	: Scene(name)
{
	fromSpline = 0.f;
}

ScenePlayer::~ScenePlayer()
{
}

void ScenePlayer::Start()
{
	//create gameobjects	
	AddChild<GameObj>("Player");
	//add & set up components and scripts	
	GetChild<GameObj>("Player")->AddScript<Player>();
	colorSpot = GetChild<GameObj>("Player")->AddComp<ColorSpot>();
	GetChild<GameObj>("Player")->AddScript<Player>()->SetHeight(fromSpline);

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
	//If got diff render pipeline

	//for (auto r : *renderables)
	//{
	//	r->Render();
	//}

	//OR

	Scene::Render();
}

void ScenePlayer::SetY(float trans)
{
	fromSpline = trans;
}
