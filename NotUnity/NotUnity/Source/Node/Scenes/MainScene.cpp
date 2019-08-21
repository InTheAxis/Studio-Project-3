#include "MainScene.h"

#include "../GameObj.h"
#include "../Components/Renderable.h"
#include "../Components/Camera.h"
#include "../Components/Sprite.h"
#include "../Scripts/DebugText.h"
#include "../Scripts/Projectile.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Input/ControllerMouse.h"
#include "../../Application.h"
#include "ScenePlayer.h"
#include "SpawnerScene.h"
#include "MapScene.h"
#include "../Scripts/PlayerController.h"

MainScene::MainScene(std::string name)
	: Scene(name)
	, debug(false)
	, gs (MENU)
	, winTimer(0)
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
	mainCam = AddChild<GameObj>("mainCam");
	AddChild<GameObj>("axes");
	AddChild<GameObj>("title");
	AddChild<GameObj>("wasd");

	//add & set up components and scripts
	//mainCam->AddComp<Camera>()->SetMode(Camera::DEBUG);
	mainCam->AddComp<Camera>()->SetMode(Camera::CUSTOM);
	mainCam->GetTransform()->translate.z = 1;
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	title = GetChild<GameObj>("title")->AddComp<Sprite>();
	title->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("title"))->SelectShader(MgrGraphics::HSV_UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	wasd = GetChild<GameObj>("wasd")->AddComp<Renderable>();
	wasd->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("wasd"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	Transform* t = GetChild<GameObj>("title")->GetTransform();
	t->translate.Set(0, 2, 0);
	t->scale.Set(3, 3, 1);
	t = GetChild<GameObj>("wasd")->GetTransform();
	t->translate.Set(-3, 1, 0);
	t->scale.Set(1.5f, 1.5f, 1);
	
	//attach camera
	GetChild<MapScene>("MapScene")->SetCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	
	mg->SetProjOrtho(88);

	Scene::Start();	

	//init variables
	spawner->SetWave(0);
	playerGO = player->GetPlayer();
}

void MainScene::Update(double dt)
{	
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	ControllerMouse* m = ControllerMouse::Instance();
	if (kb->IsKeyPressed('9'))
		debug = !debug;

	switch (gs)
	{
	case MENU:
		title->SetHSV(m_lifetime * 300, -1, -1);
		if (kb->IsKeyPressed('W') || kb->IsKeyPressed('A') || kb->IsKeyPressed('S') || kb->IsKeyPressed('D'))
			ChangeGameState(TUTO);
		break;
	case TUTO:
		if (m->IsButtonPressed(0))
			ChangeGameState(GAMEPLAY);
		break;
	case GAMEPLAY:
		if (m_lifetime > 5)	
			ChangeGameState(WIN);	
		else if (playerGO->GetScript<PlayerController>()->IsDead())
			ChangeGameState(LOSE);	
		break;
	case LOSE:
		if (!playerGO->GetScript<PlayerController>()->IsDead())
			ChangeGameState(MENU);
		break;
	case WIN:
		if (m_lifetime > winTimer + 3)
			ChangeGameState(MENU);
		break;
	}

	mainCam->GetTransform()->translate = playerGO->GetTransform()->translate;
	mainCam->GetTransform()->translate.z = 1;
	spawner->PlayerTrans(playerGO->GetTransform()->translate);
	spawner->SetTerrain(map->GetTerrain());
	player->SetTerrain(map->GetTerrain());

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

void MainScene::ChangeGameState(GAME_STATE gs)
{
	switch (this->gs)
	{
	case MENU:
		title->ActiveSelf(false);
		wasd->ActiveSelf(false);
		break;
	case TUTO:
		break;
	case GAMEPLAY:
		spawner->SetWave(0);
		break;
	case LOSE:
		break;
	case WIN:
		map->ChangeToDeSat();
		break;
	}

	switch (gs)
	{
	case MENU:
		title->ActiveSelf(true);
		wasd->ActiveSelf(true);
		//reset pos of everything
		//map->ResetPos();
		playerGO->GetScript<PlayerController>()->Reset();
		spawner->Reset();
		break;
	case TUTO:
		break;
	case GAMEPLAY:
		spawner->SetWave(1);
		break;
	case LOSE:		
		break;
	case WIN:
		map->ChangeToSaturated();
		winTimer = m_lifetime;
		break;			
	}
	this->gs = gs;
}
