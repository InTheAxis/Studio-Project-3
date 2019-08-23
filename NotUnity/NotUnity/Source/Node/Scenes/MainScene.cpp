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
#include "../Manager/MgrSound.h"

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
	spawner = nullptr;
	player = nullptr;
	map = nullptr;
	title = nullptr;
	wasd = nullptr;
	lmb = nullptr;
	playerGO = nullptr;
	mainCam = nullptr;
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
	AddChild<GameObj>("lmb");

	//add & set up components and scripts
	//mainCam->AddComp<Camera>()->SetMode(Camera::DEBUG);
	mainCam->AddComp<Camera>()->SetMode(Camera::CUSTOM);
	mainCam->GetTransform()->translate.z = 1;
	GetChild<GameObj>("axes")->AddComp<Renderable>()->AttachMesh(mg->GetCachedMesh("axes"))->AttachMaterial(mg->GetCachedMaterial("default"));
	title = GetChild<GameObj>("title")->AddComp<Sprite>();
	title->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("title"))->SelectShader(MgrGraphics::HSV_UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	wasd = GetChild<GameObj>("wasd")->AddComp<Renderable>();
	wasd->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("wasd"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	lmb = GetChild<GameObj>("lmb")->AddComp<Renderable>();
	lmb->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("lmb"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	lmb->ActiveSelf(false);
	Transform* t = GetChild<GameObj>("title")->GetTransform();
	t->translate.Set(0, 6, 0);
	t->scale.Set(4, 4, 1);
	t = GetChild<GameObj>("wasd")->GetTransform();
	t->translate.Set(-2.5f, 3, 0);
	t->scale.Set(1.5f, 1.5f, 1);
	
	//attach camera
	GetChild<MapScene>("MapScene")->SetCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());	
	mg->SetProjOrtho(Application::GetWindowHeight() * 0.12f); //divide by 720 * 88

	Scene::Start();	

	//init variables
	spawner->SetWave(0);
	playerGO = player->GetPlayer();
	
	lightAngle = 0.f;

	MgrSound::Instance()->PlayASound("bgm", true);
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
		lmb->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-2.f, 0, 0);
		break;
	case GAMEPLAY:
		if (spawner->GetEnemyKilled() >= 5 && spawner->GetBossKilled())
		{
			spawner->SetWave(spawner->GetSpawnerWave() + 1);
			spawner->NewWave();
		}
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

	lightAngle = cosf((float)m_lifetime * 2) * Math::PI * 0.1f - 1.75f;
	MgrGraphics::Instance()->SetDirLight(true, Vector3(cosf(lightAngle), sinf(lightAngle), 0));

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
		lmb->ActiveSelf(false);
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
		title->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(0, 3, 0);
		wasd->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-2.5f, 0, 0);

		//map->ResetPos();
		playerGO->GetScript<PlayerController>()->Reset();
		spawner->Reset();
		break;
	case TUTO:
		lmb->ActiveSelf(true);	
		lmb->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-2.f, 0, 0);
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
