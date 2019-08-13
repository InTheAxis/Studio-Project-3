#include "SceneExampleEmpty.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Scripts/DebugText.h"

ExampleScene::ExampleScene(std::string name)
	: Scene(name)
{
}

ExampleScene::~ExampleScene()
{
}

void ExampleScene::Start()
{	
	//create gameobjects	
	//AddChild<GameObj>("axes");
	AddChild<GameObj>("debug_text");
	//add & set up components and scripts	
	//GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("debug_text")->AddScript<DebugText>();	

	Scene::Start();
}

void ExampleScene::Update(double dt)
{	
	Scene::Update(dt);	
}

void ExampleScene::End()
{
	Scene::End();
}

void ExampleScene::Render()
{		
	//If got diff render pipeline

	//for (auto r : *renderables)
	//{
	//	r->Render();
	//}
	
	//OR

	Scene::Render();
}