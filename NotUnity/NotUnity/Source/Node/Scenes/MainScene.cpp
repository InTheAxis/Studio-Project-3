#include "MainScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "SceneExampleEmpty.h"
#include "MapScene.h"

MainScene::MainScene(std::string name)
	: Scene(name)
{
}

MainScene::~MainScene()
{
}

void MainScene::Start()
{	
	//add child scenes
	AddChild<ExampleScene>("example");
	AddChild<MapScene>("MapScene");

	//create gameobjects y
	AddChild<GameObj>("mainCam");
	AddChild<GameObj>("axes");
	//AddChild<GameObj>("debug_text");
	AddChild<GameObj>("sprite");
	//add & set up components and scripts
	GetChild<GameObj>("mainCam")->AddComp<Camera>()->SetMode(Camera::DEBUG);
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("sprite")->AddComp<Sprite>()->SetAnimation(0, 6, 1, true)->SetAnimation(1, 6, 1, true)->SwitchAnimation(0)->PlayAnimation()->AttachMesh(mg->GetCachedMesh("plane"))->AttachMaterial(mg->GetCachedMaterial("anim"));
	//GetChild<GameObj>("debug_text")->AddScript<DebugText>();
	//attach camera
	GetChild<MapScene>("MapScene")->setCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	

	Scene::Start();
}

void MainScene::Update(double dt)
{	
	if (ControllerKeyboard::Instance()->IsKeyPressed(VK_SPACE))
	{
		GetChild<GameObj>("sprite")->GetComp<Sprite>()->SwitchAnimation(1)->PlayAnimation();
	}

	Scene::Update(dt);	
}

void MainScene::End()
{
	Scene::End();
}

void MainScene::Render()
{		
	//If got diff render pipeline

	//for (auto r : *renderables)
	//{
	//	r->Render();
	//}
	
	//OR

	Scene::Render();
}