#include "MainScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Application.h"
#include "ScenePlayer.h"
#include "SpawnerScene.h"
#include "MapScene.h"

MainScene::MainScene(std::string name)
	: Scene(name)
	, debug(false)
	, gs (MENU)
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
	spawner = AddChild<SpawnerScene>("spawner");
	player = AddChild<ScenePlayer>("Player");
	map = AddChild<MapScene>("MapScene");

	//add gameobjects
	AddChild<GameObj>("mainCam");
	AddChild<GameObj>("axes");
	AddChild<GameObj>("title");
	AddChild<GameObj>("wasd");

	//add & set up components and scripts
	GetChild<GameObj>("mainCam")->AddComp<Camera>()->SetMode(Camera::DEBUG);
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	title = GetChild<GameObj>("title")->AddComp<Sprite>();
	title->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("title"))->SelectShader(MgrGraphics::HSV_UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	GetChild<GameObj>("wasd")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("wasd"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	Transform* t = GetChild<GameObj>("title")->GetTransform();
	t->translate.Set(0, 2, 0);
	t->scale.Set(3, 3, 1);
	t = GetChild<GameObj>("wasd")->GetTransform();
	t->translate.Set(-3, 1, 0);
	t->scale.Set(3, 3, 1);
	
	//attach camera
	GetChild<MapScene>("MapScene")->setCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	
	mg->SetProjOrtho(128);

	//init variables
	spawner->SetWave(1);

	Scene::Start();
}

void MainScene::Update(double dt)
{	
	if (ControllerKeyboard::Instance()->IsKeyPressed('9'))
		debug = !debug;

	switch (gs)
	{
	case GAME_STATE::MENU:
		title->SetHSV(m_lifetime * 300, -1, -1);
		break;
	}

	spawner->PlayerTrans(player->GetChild<GameObj>("Player")->GetTransform()->translate);

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
	if (debug)
		RenderPass(RENDER_PASS::FINAL);
}