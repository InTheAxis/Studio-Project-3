#include "SceneExampleEmpty.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/ColorSpot.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../Manager/MgrGameObj.h"

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

	AddChild<GameObj>("sat")->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("anim"))->SelectShader(MgrGraphics::HSV);
	GetChild<GameObj>("sat")->GetComp<Sprite>()->SetHSV(-1, 1, -1)->SetRenderPass(RENDER_PASS::POST_FX);
	GetChild<GameObj>("sat")->GetTransform()->translate.y = 1.f;


	AddChild<GameObj>("testSprite1")->AddComp<Sprite>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("sattest"))->SelectShader(MgrGraphics::DEFAULT);
	AddChild<GameObj>("colorSpot")->AddComp<ColorSpot>();	
	AddChild<GameObj>("colorSpot2")->AddComp<ColorSpot>();	

	Scene::Start();
}

void ExampleScene::Update(double dt)
{
	if (ControllerKeyboard::Instance()->IsKeyDown(VK_SPACE))
	{
		GetChild<GameObj>("sat")->GetComp<Sprite>()->SetHSV(-1, GetChild<GameObj>("sat")->GetComp<Sprite>()->GetHSV().y - 0.01f, -1);
	}

	if (ControllerKeyboard::Instance()->IsKeyDown(VK_UP))
	{
		GetChild<GameObj>("colorSpot")->GetTransform()->translate.y += 0.5f * dt;
	}

	if (ControllerKeyboard::Instance()->IsKeyDown(VK_DOWN))
	{
		GetChild<GameObj>("colorSpot")->GetTransform()->translate.y -= 0.5f * dt;
	}
	if (ControllerKeyboard::Instance()->IsKeyDown(VK_LEFT))
	{
		GetChild<GameObj>("colorSpot")->GetTransform()->translate.x -= 0.5f * dt;
	}
	if (ControllerKeyboard::Instance()->IsKeyDown(VK_RIGHT))
	{
		GetChild<GameObj>("colorSpot")->GetTransform()->translate.x += 0.5f * dt;
	}

	GetChild<GameObj>("colorSpot")->GetComp<ColorSpot>()->SetUniform(0);
	GetChild<GameObj>("colorSpot2")->GetComp<ColorSpot>()->SetUniform(1);

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