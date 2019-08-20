#include "ScenePlayer.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/Player.h"
#include "../Scripts/PlayerController.h"
#include "../Scripts/ColorSpot.h"
#include "../Scripts/SkillTree.h"
#include "../../Node/Scripts/Achievements.h"
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
	AddChild<GameObj>("Player");
	
	//add & set up components and scripts	
	playerScript = GetChild<GameObj>("Player")->AddScript<PlayerController>();
	AddChild("SkillTree", SkillTree::Instance());
	AddChild("Achievement", Achievements::Instance());
	
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
	//If got diff render pipeline

	//for (auto r : *renderables)
	//{
	//	r->Render();
	//}

	//OR

	Scene::Render();
}

GameObj* ScenePlayer::GetPlayer()
{
	return playerScript->GetGameObj();
}

void ScenePlayer::SetTerrainHeight(float h)
{
	playerScript->SetHeight(h);
}
