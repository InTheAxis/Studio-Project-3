#include "MgrScene.h"

#include "../Scene.h"

MgrScene::MgrScene(std::string name) : Manager<MgrScene>(name)
{
}

MgrScene::~MgrScene()
{
}

void MgrScene::LoadScenes()
{
	allScenes["default"] = new Scene;
	SwitchScene("default");
}

void MgrScene::Start()
{	
	LoadScenes();
	Node::Start();
}

void MgrScene::Update(double dt)
{	
	rootScene->Update(dt);
	Node::Update(dt);
}

void MgrScene::End()
{
	rootScene->End();
	Node::End();
}

Scene * MgrScene::GetCurrScene() const
{
	return currScene;
}

void MgrScene::SetCurrScene(Scene * s)
{
	currScene = s;
}

Scene * MgrScene::SwitchScene(std::string name)
{
	rootScene = allScenes[name];
	rootScene->Start();
	return rootScene;
}

