#include "Scene.h"
#include "GameObj.h"

#include <vector>
#include <sstream>

#include "Manager/MgrGraphics.h"
#include "Manager/MgrScene.h"
#include "Manager/MgrGameObj.h"
#include "Components/Camera.h"
#include "Components/Renderable.h"
#include "Components/Text.h"
#include "Components/Sprite.h"
#include "Scripts/DebugText.h"
#include "GameObj.h"
#include "../Utility/Input/ControllerKeyboard.h"

Scene::Scene(std::string name) 
	: Node(name)
	, mg(nullptr)
	, mgo(nullptr)
	, renderables(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::Start()
{		
	//set current scene
	MgrScene::Instance()->SetCurrScene(this);
	//get singleton references
	mg = MgrGraphics::Instance();
	mgo = MgrGameObj::Instance();
	//get go list references
	renderables = mgo->GetRenderables();

	//create gameobjects
	AddChild<GameObj>("mainCam");
	AddChild<GameObj>("axes");
	AddChild<GameObj>("debug_text");
	AddChild<GameObj>("sprite");
	//add & set up components and scripts
	GetChild<GameObj>("mainCam")->AddComp<Camera>()->SetMode(Camera::DEBUG);
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	GetChild<GameObj>("sprite")->AddComp<Sprite>()->SetAnimation(0, 6, 1, true)->SetAnimation(1, 6, 1, true)->SwitchAnimation(0)->PlayAnimation()->AttachMesh(mg->GetCachedMesh("plane"))->AttachMaterial(mg->GetCachedMaterial("anim"));
	GetChild<GameObj>("debug_text")->AddScript<DebugText>();
	//attach camera
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	

	Node::Start();
}

void Scene::Update(double dt)
{	
	if (ControllerKeyboard::Instance()->IsKeyPressed(VK_SPACE))
	{
		GetChild<GameObj>("sprite")->GetComp<Sprite>()->SwitchAnimation(1)->PlayAnimation();
	}

	Node::Update(dt);
	this->Render();
}

void Scene::End()
{
	Node::End();
}

void Scene::Render()
{		
	for (auto r : *renderables)
	{
		r->Render();
	}
}