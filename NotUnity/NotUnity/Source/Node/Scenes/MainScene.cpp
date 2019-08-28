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
#include "../Manager/MgrMain.h"
#include "../Manager/MgrAchievements.h"
#include "../Components/Text.h"

MainScene::MainScene(std::string name)
	: Scene(name)
	, debug(false)
	, gs(MENU)
	, winTimer(0)
	, pause(false)
	, walkSat(0.f)
	, attSat(0.f)
	, killSat(0.f)
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
	pauseMenu = nullptr;
	walkAchievement = nullptr;
	walkAchievement = nullptr;
	playerGO = nullptr;
	mainCam = nullptr;
	waveNum = nullptr;
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
	AddChild<GameObj>("pauseMenu");
	AddChild<GameObj>("greenbar");
	AddChild<GameObj>("redbar");
	AddChild<GameObj>("walkAchievement");
	AddChild<GameObj>("attackAchievement");
	AddChild<GameObj>("killAchievement");
	AddChild<GameObj>("waveCounter");
	AddChild<GameObj>("walkText");
	AddChild<GameObj>("debugText")->AddComp<DebugText>();

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

	pauseMenu = GetChild<GameObj>("pauseMenu")->AddComp<Renderable>();
	pauseMenu->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("paused"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	pauseMenu->ActiveSelf(false);

	// Set Text for pause screen
	textWalkAchievement = GetChild<GameObj>("walkText")->AddComp<Text>();
	textWalkAchievement->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("text"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("font"))->SetRenderPass(RENDER_PASS::HUD);
	textWalkAchievement->SetSize(0.5f);
	textWalkAchievement->ActiveSelf(false);
	textWalkAchievement->GetGameObj()->GetTransform()->translate = Vector3(-9.5f, 1.5f, 0.2f);
	ss.clear(); ss.str(""); ss << "Attack Damage I        Walk Speed I          Health Increase I";
	textWalkAchievement->SetText(ss.str());

	//textAttackAchievement = GetChild<GameObj>("attackText")->AddComp<Text>();
	//textAttackAchievement->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("text"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("font"))->SetRenderPass(RENDER_PASS::HUD);
	//textAttackAchievement->SetSize(0.5f);
	//textAttackAchievement->ActiveSelf(false);
	//textAttackAchievement->GetGameObj()->GetTransform()->translate = Vector3(-9.5f, 1.f, 0.2f);
	//ss.clear(); ss.str(""); ss << "Attack Damage I";
	//textAttackAchievement->SetText(ss.str());

	waveNum = GetChild<GameObj>("waveCounter")->AddComp<Text>();
	waveNum->AttachMesh(MgrGraphics::Instance()->GetCachedMesh("text"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("font"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	waveNum->SetSize(2.f);
	GetChild<GameObj>("waveCounter")->GetComp<Transform>()->translate.Set(0, 5, -3);
	waveNum->SetAlignment(0);
	waveNum->ActiveSelf(false);


	// Set the sprite for the achievement
	walkAchievement = GetChild<GameObj>("walkAchievement")->AddComp<Sprite>();
	walkAchievement->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("achievements"))->SelectShader(MgrGraphics::HSV_UNLIT)->SetRenderPass(RENDER_PASS::HUD)->ActiveSelf(false);

	attackAchievemnt = GetChild<GameObj>("attackAchievement")->AddComp<Sprite>();
	attackAchievemnt->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("achievements"))->SelectShader(MgrGraphics::HSV_UNLIT)->SetRenderPass(RENDER_PASS::HUD)->ActiveSelf(false);

	killAchievement = GetChild<GameObj>("killAchievement")->AddComp<Sprite>();
	killAchievement->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("achievements"))->SelectShader(MgrGraphics::HSV_UNLIT)->SetRenderPass(RENDER_PASS::HUD)->ActiveSelf(false);

	greenbar = GetChild<GameObj>("greenbar")->AddComp<Renderable>();
	greenbar->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("greenbar"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	greenbar->ActiveSelf(false);
	redbar = GetChild<GameObj>("redbar")->AddComp<Renderable>();
	redbar->AttachMesh(mg->GetCachedMesh("quad"))->AttachMaterial(mg->GetCachedMaterial("redbar"))->SelectShader(MgrGraphics::UNLIT)->SetRenderPass(RENDER_PASS::HUD);
	redbar->ActiveSelf(false);
	Transform* t = GetChild<GameObj>("title")->GetTransform();
	t->scale.Set(4, 4, 1);
	t = GetChild<GameObj>("wasd")->GetTransform();
	t->scale.Set(1.5f, 1.5f, 1);
	t = GetChild<GameObj>("pauseMenu")->GetTransform();
	t->scale.Set(16.f, 9.f, 1);

	//attach camera
	GetChild<MapScene>("MapScene")->SetCamera(GetChild<GameObj>("mainCam")->GetComp<Camera>());
	mg->AttachView(GetChild<GameObj>("mainCam")->GetComp<Camera>()->GetViewMtx());
	mg->SetProjOrtho(Application::GetWindowHeight() * 0.12f); //divide by 720 * 88

	Scene::Start();

	//init variables
	spawner->SetWave(1);
	spawner->SetStartGame(false);
	playerGO = player->GetPlayer();
	player->SetCameraRef(mainCam->GetComp<Camera>());

	healthminus = 1;
	lightAngle = 0.f;

	MgrSound::Instance()->PlayASound("bgm", true);
}

void MainScene::Update(double dt)
{
	ControllerKeyboard* kb = ControllerKeyboard::Instance();
	ControllerMouse* m = ControllerMouse::Instance();
	if (kb->IsKeyPressed('9'))
		debug = !debug;

	//if (kb->IsKeyDown('F'))
	//	healthminus -= 0.01 * dt;
	switch (gs)
	{
	case MENU:
		wasd->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-1.5f, 0, 0);
		title->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(0, 2, 0);
		title->SetHSV((float)m_lifetime * 300, -1, -1);
		if (kb->IsKeyPressed('W') || kb->IsKeyPressed('A') || kb->IsKeyPressed('S') || kb->IsKeyPressed('D'))
			ChangeGameState(TUTO);
		break;
	case TUTO:
		if (kb->IsKeyPressed(VK_TAB))
			pause = !pause;
		if (m->IsButtonPressed(0))
			ChangeGameState(GAMEPLAY);
		lmb->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-1.5f, 0, 0);
		break;
	case GAMEPLAY:
		if (kb->IsKeyPressed(VK_TAB))
			pause = !pause;
		greenbar->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3((player->GetHealth() * 0.05f) - 7.2f, 4.0f, 0.f);
		greenbar->GetGameObj()->GetTransform()->scale.Set(player->GetHealth() * 0.125f, 1, 0);
		redbar->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-6.2f, 4.0f, 0);
		redbar->GetGameObj()->GetTransform()->scale.Set(2.4f, 1, 0);

		spawner->SetStartGame(true);
		if (spawner->GetSpawnerWave() < 5)
		{
			if (spawner->GetEnemyKilled() >= 6 && spawner->GetBossKilled())
			{
				MgrAchievements::Instance()->SetEnemyKilled(spawner->GetEnemyKilled());
				MgrAchievements::Instance()->ResetEnemyKilled();
				spawner->SetWave(spawner->GetSpawnerWave() + 1);
				spawner->Reset();
				playerGO->GetScript<PlayerController>()->ResetHealth();
			}
		}
		else if (spawner->GetSpawnerWave() >= 5 && spawner->GetEnemyKilled() >= 6 && spawner->GetBossKilled())
			ChangeGameState(WIN);
		if (playerGO->GetScript<PlayerController>()->IsDead())
			ChangeGameState(LOSE);

		waveNum->ActiveSelf(true);
		ss.clear(); ss.str(""); ss.precision(2); ss << spawner->GetSpawnerWave();
		waveNum->SetText(ss.str());
		player->SetColorSpotRad(spawner->GetWave() / 5.f * 9 + 1);
		break;
	case LOSE:
		if (!playerGO->GetScript<PlayerController>()->IsDead())
			ChangeGameState(MENU);
		MgrAchievements::Instance()->SetFinalKilled();
		break;
	case WIN:
		if (m_lifetime > winTimer + 3)
			ChangeGameState(MENU);
		break;
	}

	pauseMenu->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(0, 0, 0.1f);
	pauseMenu->ActiveSelf(pause);

	updateAchievementUI();

	walkAchievement->SetHSV(-1.f, walkSat, -1.f);
	walkAchievement->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(0, 0, 0.2f);

	attackAchievemnt->SetHSV(-1.f, attSat, -1.f);
	attackAchievemnt->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-4.f, 0, 0.2f);

	killAchievement->SetHSV(-1.f, killSat, -1.f);
	killAchievement->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(4.f, 0, 0.2f);

	walkAchievement->ActiveSelf(pause);
	attackAchievemnt->ActiveSelf(pause);
	killAchievement->ActiveSelf(pause);

	textWalkAchievement->ActiveSelf(pause);
	
	MgrMain::Instance()->SetTimeScale((float)!pause);
	if (pause) return;

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
		waveNum->ActiveSelf(false);
		spawner->Reset();
		spawner->SetStartGame(false);
		spawner->SetWave(0);
		player->SetColorSpotRad(1);
		break;
	case TUTO:
		lmb->ActiveSelf(false);
		waveNum->ActiveSelf(false);
		break;
	case GAMEPLAY:
		greenbar->ActiveSelf(false);
		greenbar->ActiveSelf(false);
		redbar->ActiveSelf(false);
		waveNum->ActiveSelf(false);
		spawner->Reset();
		spawner->SetStartGame(false);
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
		waveNum->ActiveSelf(false);
		spawner->SetStartGame(false);
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
		greenbar->ActiveSelf(true);
		redbar->ActiveSelf(true);
		waveNum->ActiveSelf(true);
		greenbar->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3((player->GetHealth() * 0.05f) - 7.2f, 4.0f, 0.f);
		redbar->GetGameObj()->GetTransform()->translate = playerGO->GetTransform()->translate + Vector3(-6.3f, 4.0f, 0);
		spawner->SetWave(1);
		spawner->SetStartGame(true);
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

void MainScene::updateAchievementUI()
{
	if (!MgrAchievements::Instance()->GetWalkAchievementLevel1())
	{
		walkSat = MgrAchievements::Instance()->GetWalkTime() / 10;
	}
	else if (!MgrAchievements::Instance()->GetWalkAchievementLevel2())
	{
		walkSat = MgrAchievements::Instance()->GetWalkTime() / 100;
		ss.clear(); ss.str(""); ss << "Attack Damage I        Walk Speed II          Health Increase I"; textWalkAchievement->SetText(ss.str());
	}
	else
	{
		ss.clear(); ss.str(""); ss << "Attack Damage I        Walk Speed III          Health Increase I"; textWalkAchievement->SetText(ss.str());
	}
	if (!MgrAchievements::Instance()->GetAttackLevel1())
	{
		attSat = MgrAchievements::Instance()->GetAttactTimes() / 10;
	}
	else if (!MgrAchievements::Instance()->GetAttackLevel2())
	{
		attSat = MgrAchievements::Instance()->GetAttactTimes() / 50;
		ss.clear(); ss.str(""); ss << "Attack Damage II        Walk Speed I          Health Increase I"; textWalkAchievement->SetText(ss.str());
	}
	else
	{
		ss.clear(); ss.str(""); ss << "Attack Damage III        Walk Speed I          Health Increase I"; textWalkAchievement->SetText(ss.str());
	}
	if (!MgrAchievements::Instance()->GetKillLevel1())
	{
		killSat = MgrAchievements::Instance()->GetFinalEnemyKilled() / 10;
	}
	else if (!MgrAchievements::Instance()->GetKillLevel2())
	{
		killSat = MgrAchievements::Instance()->GetFinalEnemyKilled() / 50;
		ss.clear(); ss.str(""); ss << "Attack Damage I        Walk Speed I          Health Increase II"; textWalkAchievement->SetText(ss.str());
	}
	else
	{
		ss.clear(); ss.str(""); ss << "Attack Damage I        Walk Speed I          Health Increase III"; textWalkAchievement->SetText(ss.str());
	}
	if (MgrAchievements::Instance()->GetAttackLevel2() && MgrAchievements::Instance()->GetWalkAchievementLevel2())
	{
		ss.clear(); ss.str(""); ss << "Attack Damage III        Walk Speed III          Health Increase I"; textWalkAchievement->SetText(ss.str());
	}
	else if (MgrAchievements::Instance()->GetAttackLevel2() && MgrAchievements::Instance()->GetKillLevel2())
	{
		ss.clear(); ss.str(""); ss << "Attack Damage III        Walk Speed I          Health Increase III"; textWalkAchievement->SetText(ss.str());
	}
	else if (MgrAchievements::Instance()->GetWalkAchievementLevel2() && MgrAchievements::Instance()->GetKillLevel2())
	{
		ss.clear(); ss.str(""); ss << "Attack Damage I        Walk Speed III          Health Increase III"; textWalkAchievement->SetText(ss.str());
	}
	else if (MgrAchievements::Instance()->GetAttackLevel2() && MgrAchievements::Instance()->GetWalkAchievementLevel2() && MgrAchievements::Instance()->GetKillLevel2())
	{
		ss.clear(); ss.str(""); ss << "Attack Damage III        Walk Speed III          Health Increase III"; textWalkAchievement->SetText(ss.str());
	}
}