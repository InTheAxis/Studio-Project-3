#include "MainScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "SceneExampleEmpty.h"
#include "ScenePlayer.h"
#include "../../Application.h"
#include "MapScene.h"

MainScene::MainScene(std::string name)
	: Scene(name)
{
	floatFbo[0].Init(Application::GetWindowWidth(), Application::GetWindowHeight());
	floatFbo[1].Init(Application::GetWindowWidth(), Application::GetWindowHeight());
}

MainScene::~MainScene()
{
}

void MainScene::Start()
{	
	//creating quads to render framebuffers	
	AddChild<GameObj>("fbo")->AddComp<Renderable>()->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("quad"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("fbo"))->SelectShader(MgrGraphics::SIMPLE)->SetRenderPass(RENDER_PASS::MANUAL);
	GetChild<GameObj>("fbo")->GetTransform()->scale.Set(2, 2, 1);
	
	//add child scenes
	AddChild<ExampleScene>("example");
	AddChild<ScenePlayer>("Player");
	AddChild<MapScene>("MapScene");

	//create gameobjects y
	AddChild<GameObj>("mainCam");
	AddChild<GameObj>("axes");
	//AddChild<GameObj>("debug_text");
	//AddChild<GameObj>("sprite");
	//add & set up components and scripts
	GetChild<GameObj>("mainCam")->AddComp<Camera>()->SetMode(Camera::DEBUG);
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	//GetChild<GameObj>("sprite")->AddComp<Sprite>()->SetAnimation(0, 6, 1, true)->SetAnimation(1, 6, 1, true)->SwitchAnimation(0)->PlayAnimation()->AttachMesh(mg->GetCachedMesh("plane"))->AttachMaterial(mg->GetCachedMaterial("anim"));
	//GetChild<GameObj>("debug_text")->AddScript<DebugText>();
	//attach camera
	GetChild<MapScene>("MapScene")->setCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	
	//mg->SetProjOrtho();

	Scene::Start();
}

void MainScene::Update(double dt)
{	
	if (ControllerKeyboard::Instance()->IsKeyPressed(VK_SPACE))
	{
		//GetChild<GameObj>("sprite")->GetComp<Sprite>()->SwitchAnimation(1)->PlayAnimation();
	}

	Scene::Update(dt);	
}

void MainScene::End()
{
	Scene::End();
}

void MainScene::Render()
{		
	MgrGraphics* mgrG = MgrGraphics::Instance();
	Renderable* fbo = GetChild<GameObj>("fbo")->GetComp<Renderable>();	
	
	floatFbo[0].BindForWriting();
	mgrG->PreRender();
	RenderPass(RENDER_PASS::GEO);

	floatFbo[1].BindForWriting();
	floatFbo[0].BindForReading(GL_TEXTURE0);
	mgrG->GetCachedMaterial("fbo")->maps[0] = floatFbo[0].GetTexture();
	mgrG->PreRender();
	glDepthMask(GL_FALSE); //ignore depth
	fbo->SelectShader(mgrG->COLOR_SPOT)->Render();
	RenderPass(RENDER_PASS::POST_FX);
	glDepthMask(GL_TRUE); // to clear it
	
	floatFbo[0].BindForWriting();
	floatFbo[1].BindForReading(GL_TEXTURE0);
	mgrG->GetCachedMaterial("fbo")->maps[0] = floatFbo[1].GetTexture();
	mgrG->PreRender();
	glDepthMask(GL_FALSE);
	fbo->SelectShader(mgrG->SIMPLE)->Render();
	RenderPass(RENDER_PASS::HUD);
	glDepthMask(GL_TRUE);

	FBO::BindDefault();
	floatFbo[0].BindForReading(GL_TEXTURE0);
	mgrG->GetCachedMaterial("fbo")->maps[0] = floatFbo[0].GetTexture();
	mgrG->PreRender();
	fbo->SelectShader(mgrG->SIMPLE)->Render();
	RenderPass(RENDER_PASS::FINAL);
	
}