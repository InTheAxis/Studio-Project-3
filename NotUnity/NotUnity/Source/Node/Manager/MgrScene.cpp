#include "MgrScene.h"
#include "MgrGameObj.h"
#include "MgrGraphics.h"

#include "../Scenes/MainScene.h"
#include "../Scenes/SpawnerScene.h"
#include "../Scenes/ScenePlayer.h"
#include "../../Utility/Input/ControllerKeyboard.h"

MgrScene::MgrScene(std::string name) : Manager<MgrScene>(name)
{
}

MgrScene::~MgrScene()
{
	rootScene = nullptr;
	for (auto s : allScenes)
	{
		if (s.second)
			delete s.second;
		s.second = nullptr;
	}
	allScenes.clear();
}

void MgrScene::LoadScenes()
{
	Debug::Log("Loading scenes...");
	allScenes["default"] = new MainScene;	
	//allScenes["spawner"] = new SpawnerScene;
	//allScenes["player"] = new ScenePlayer;
	
	SwitchScene("default"); //change this line to your working scene
}

void MgrScene::Start()
{	
	Math::InitRNG();
	LoadScenes();
	Node::Start();
}

void MgrScene::Update(double dt)
{		
	rootScene->Update(dt);
	rootScene->Render();
	Node::Update(dt);
}

void MgrScene::End()
{
	rootScene->End();
	Node::End();
}

Scene * MgrScene::GetRoot() const
{
	return rootScene;
}

Scene * MgrScene::SwitchScene(std::string name)
{	
	//TODO, DOESNT WORK, NEED DO CLEANUP IN END()
	if (rootScene) rootScene->End();
	rootScene = allScenes[name];
	rootScene->Start();
	return rootScene;
}

