#include "MainScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "SceneExampleEmpty.h"
#include "../../Application.h"
#include "spawnerScene.h"

MainScene::MainScene(std::string name)
	: Scene(name)
{
	fbo.Init(Application::GetWindowWidth(), Application::GetWindowHeight());
}

MainScene::~MainScene()
{
}

void MainScene::Start()
{	
	//creating quads to render framebuffers	
	MgrGraphics::Instance()->GetCachedMaterial("fbo")->maps[0] = fbo.GetTexture();
	AddChild<GameObj>("fbo")->AddComp<Renderable>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("fbo"))->SelectShader(MgrGraphics::SIMPLE);
	GetChild<GameObj>("fbo")->GetTransform()->scale.Set(2, 2, 1);
	
	//add child scenes
	AddChild<ExampleScene>("example");
	AddChild<SpawnerScene>("spawner")->setWave(1);

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
	fbo.BindForWriting();
	MgrGraphics::Instance()->PreRender();
	for (auto r : *renderables)
	{		
		if (r != GetChild<GameObj>("fbo")->GetComp<Renderable>())
			r->Render();
	}
	FBO::BindDefault();
	fbo.BindForReading(GL_TEXTURE0);
	MgrGraphics::Instance()->PreRender();
	GetChild<GameObj>("fbo")->GetComp<Renderable>()->Render();
	/*for (auto r : *renderables)
	{
		r->Render();
	}*/
}