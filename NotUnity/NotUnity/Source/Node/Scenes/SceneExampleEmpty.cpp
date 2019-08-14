#include "SceneExampleEmpty.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../../Utility/Input/ControllerKeyboard.h"

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

	AddChild<GameObj>("sat")->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("sattest"))->SelectShader(MgrGraphics::HSV);
	GetChild<GameObj>("sat")->GetComp<Sprite>()->SetHSV(-1, 0, -1);

	Scene::Start();
}

void ExampleScene::Update(double dt)
{
	if (ControllerKeyboard::Instance()->IsKeyDown(VK_SPACE))
	{
		GetChild<GameObj>("sat")->GetComp<Sprite>()->SetHSV(-1, GetChild<GameObj>("sat")->GetComp<Sprite>()->GetHSV().y + 0.01f, -1);
	}
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