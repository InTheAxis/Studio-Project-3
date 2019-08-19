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
#include "spawnerScene.h"
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
	AddChild<MapScene>("MapScene");
	scenePlayer = AddChild<ScenePlayer>("Player");
	sceneSpawner = AddChild<SpawnerScene>("spawner");
	sceneSpawner->SetWave(1);

	AddChild<GameObj>("mainCam");
	AddChild<GameObj>("axes");
	//add & set up components and scripts
	GetChild<GameObj>("mainCam")->AddComp<Camera>()->SetMode(Camera::DEBUG);
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	//attach camera
	GetChild<MapScene>("MapScene")->setCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	
	//mg->SetProjOrtho(128);
	Scene::Start();
}

void MainScene::Update(double dt)
{	
	sceneSpawner->PlayerTrans(scenePlayer->GetChild<GameObj>("Player")->GetTransform()->translate);
	//scenePlayer->SetY(spCubic->Fn(scenePlayer->GetChild<GameObj>("Player")->GetTransform()->translate.x));
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
	glDepthMask(GL_TRUE); // to clear it
	RenderPass(RENDER_PASS::POST_FX);
	
	floatFbo[0].BindForWriting();
	floatFbo[1].BindForReading(GL_TEXTURE0);
	mgrG->GetCachedMaterial("fbo")->maps[0] = floatFbo[1].GetTexture();
	mgrG->PreRender();
	glDepthMask(GL_FALSE);
	fbo->SelectShader(mgrG->SIMPLE)->Render();
	glDepthMask(GL_TRUE);
	RenderPass(RENDER_PASS::HUD);

	FBO::BindDefault();
	floatFbo[0].BindForReading(GL_TEXTURE0);
	mgrG->GetCachedMaterial("fbo")->maps[0] = floatFbo[0].GetTexture();
	mgrG->PreRender();
	glDepthMask(GL_FALSE);
	fbo->SelectShader(mgrG->SIMPLE)->Render();
	glDepthMask(GL_TRUE);
	RenderPass(RENDER_PASS::FINAL);
}