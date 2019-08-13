#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"
#include "RenderHelper.h"
#include "FPSCounter.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "WeaponInfo/AmmoDumpManager.h"
#include "Minimap\Minimap.h"
#include "Environment/Furniture.h"

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
	: theCameraEffects(NULL)
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
	: theCameraEffects(NULL)
{
	_sceneMgr->AddScene("Start", this);
}

void SceneText::ChangePlayer(int i)
{
	playerInfo[currPlayer]->Reset();
	currPlayer = i;
	camera.Init(playerInfo[i]->GetPos(), playerInfo[i]->GetTarget(), playerInfo[i]->GetUp());
	playerInfo[i]->AttachCamera(&camera);
	CMinimap::GetInstance()->Init(playerInfo[currPlayer]);
	theKeyboard->Create(playerInfo[i]);	
	theMouse->Create(playerInfo[i]);
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair", 1, 0, 0, 10);
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);	
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 0.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//gdev//grey.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//gdev//white.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA( "Image//SkyBox//sky0_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA(  "Image//SkyBox//sky0_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA(  "Image//SkyBox//sky0_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA( "Image//SkyBox//sky0_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA(   "Image//SkyBox//sky0_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//sky0_bottom.tga");
	
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//gdev//red.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("PLAYER_PISTOL", "OBJ//gdev//pistol.obj");
	MeshBuilder::GetInstance()->GetMesh("PLAYER_PISTOL")->textureID = LoadTGA("Image//gdev//black.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLAYER_HOOK", "OBJ//gdev//hook.obj");
	MeshBuilder::GetInstance()->GetMesh("PLAYER_HOOK")->textureID = LoadTGA("Image//gdev//black.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLAYER_ASSAULT", "OBJ//gdev//ak47.obj");
	MeshBuilder::GetInstance()->GetMesh("PLAYER_ASSAULT")->textureID = LoadTGA("Image//gdev//black.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("PLAYER_SHOTGUN", "OBJ//gdev//shotgun.obj");
	MeshBuilder::GetInstance()->GetMesh("PLAYER_SHOTGUN")->textureID = LoadTGA("Image//gdev//black.tga");

	MeshBuilder::GetInstance()->GenerateCube("AMMO_DUMP", Color(1.0f, 1.0f, 0.0f), 3.0f);
	MeshBuilder::GetInstance()->GetMesh("AMMO_DUMP")->textureID = LoadTGA("Image//gdev//blue.tga");

	MeshBuilder::GetInstance()->GenerateCube("PROJECTILE", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("PROJECTILE")->textureID = LoadTGA("Image//gdev//dark red.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("ENEMY", "OBJ//gdev//enemy.obj");
	MeshBuilder::GetInstance()->GetMesh("ENEMY")->textureID = LoadTGA("Image//gdev//red.tga");

	MeshBuilder::GetInstance()->GenerateQuad("HP", Color(1, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("HP")->textureID = LoadTGA("Image//gdev//red.tga");

	MeshBuilder::GetInstance()->GenerateQuad("clip", Color(1, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("clip")->textureID = LoadTGA("Image//gdev//blue.tga");

	MeshBuilder::GetInstance()->GenerateQuad("mag", Color(1, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("mag")->textureID = LoadTGA("Image//gdev//black.tga");

	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("pistol", "Image//gdev//pistol.mp3");
	CSoundEngine::GetInstance()->AddSound("shotgun", "Image//gdev//shotgun.mp3");
	CSoundEngine::GetInstance()->AddSound("woosh", "Image//gdev//woosh.mp3");
	CSoundEngine::GetInstance()->AddSound("pop", "Image//gdev//pop.mp3");
	CSoundEngine::GetInstance()->AddSound("superhot", "Image//gdev//superhot.mp3");
	CSoundEngine::GetInstance()->AddSound("reload", "Image//gdev//reload.mp3");
	CSoundEngine::GetInstance()->AddSound("noammo", "Image//gdev//noammo.mp3");
	CSoundEngine::GetInstance()->AddSound("slap", "Image//gdev//slap.mp3");	

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));	

	// Create the playerinfo instance, which manages all information about the player
	currPlayer = 0;
	for (int i = 0; i < NUM_PLAYER; ++i)
	{
		playerInfo[i] = new CPlayerInfo;
		playerInfo[i]->Init(i);
		playerInfo[i]->ResetHealth();
	}

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	//camera.Init(playerInfo[0]->GetPos(), playerInfo[0]->GetTarget(), playerInfo[0]->GetUp());
	//playerInfo[0]->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 100.0f), Vector3(0.5f, 0.5f, 0.5f), true);

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	for (int i = 0; i < NUM_PLAYER; ++i)
		playerInfo[i]->SetTerrain(groundEntity);

	// Create a CEnemy instance
	for (int i = 0; i < NUM_ENEMY; ++i)
	{
		anEnemy3D[i] = Create::Enemy3D("ENEMY", Vector3(i, 0.0f, -50.0f * (i + 1)), Vector3(1.5f, 1.5f, 1.5f));
		anEnemy3D[i]->Init();
		anEnemy3D[i]->SetTerrain(groundEntity);
	}

	aChair = Create::Furniture("Chair", Vector3(-50.0f, 0.0f, -20.0f));
	aChair->SetTerrain(groundEntity);

	// Initialise the ammo dumps
	CAmmoDumpManager::GetInstance()->Init();

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 30.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth+fontSize, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0,0,0));
	}
	textObj[0]->SetText("HELLO WORLD");

	// Activate the Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Minimap
	CMinimap* theMinimap = CMinimap::GetInstance();	
	theMinimap->SetBackground(MeshBuilder::GetInstance()->GenerateQuad("MINIMAP", Color(1, 1, 1), 1.f));
	theMinimap->GetBackground()->textureID = LoadTGA("Image//gdev//minimap.tga");
	theMinimap->SetBorder(MeshBuilder::GetInstance()->GenerateCircle("MINIMAPBORDER", Color(1, 1, 1), 1.1f));
	theMinimap->GetBorder()->textureID = LoadTGA("Image//gdev//black.tga");
	theMinimap->SetAvatar(MeshBuilder::GetInstance()->GenerateQuad("MINIMAPAVATAR", Color(1, 1, 0), 0.25f));
	theMinimap->GetAvatar()->textureID = LoadTGA("Image//gdev//avatar.tga");
	theMinimap->SetStencil(MeshBuilder::GetInstance()->GenerateCircle("MINIMAP_STENCIL", Color(1, 1, 1), 1.0f));
	theMinimap->SetTarget(MeshBuilder::GetInstance()->GenerateQuad("REDDOT", Color(1, 0, 0), 0.05f));
	theMinimap->SetEnemies(anEnemy3D, NUM_ENEMY);

	// CameraEffects
	theCameraEffects = Create::CameraEffects(false);
	theCameraEffects->SetBloodScreen(MeshBuilder::GetInstance()->GenerateQuad("CAMERAEFFECTS_BLOODSCREEN", 
										Color(1, 1, 1), 1.f));
	theCameraEffects->GetBloodScreen()->textureID = LoadTGA("Image//gdev//scope.tga");
	theCameraEffects->SetStatus_BloodScreen(false);

	// Hardware Abstraction
	theKeyboard = new CKeyboard();
	//theKeyboard->Create(playerInfo[0]);

	theMouse = new CMouse();
	//theMouse->Create(playerInfo[0]);

	ChangePlayer(0);

	timeScale = 1.f;

	score = 0;

	CSoundEngine::GetInstance()->Play3dSound("superhot", Vector3());
}

void SceneText::Update(double dt)
{		
	
	CSoundEngine::GetInstance()->SetListenerPos(playerInfo[currPlayer]->GetPos(), playerInfo[currPlayer]->GetView());

	if (KeyboardController::GetInstance()->IsKeyDown('E')) 
	{
		debugTimeScale = true;		
	}
	else
		debugTimeScale = false;

	timeScale = debugTimeScale ? 1 : playerInfo[currPlayer]->GetMovementSpeed();

	theCameraEffects->SetStatus_BloodScreen(playerInfo[currPlayer]->GetShowScope());

	int count = 0;
	for (int i = 0; i < NUM_ENEMY; ++i)
	{
		anEnemy3D[i]->UpdatePlayerPos(playerInfo[currPlayer]->GetPos());
		if (anEnemy3D[i]->IsDone())
		{
			++count;
		}
	}
	score = count * 100;	
	// Update our entities
	EntityManager::GetInstance()->Update(dt * timeScale);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		if (CMinimap::GetInstance()->GetZoom() > 1)
			CMinimap::GetInstance()->SetZoom(CMinimap::GetInstance()->GetZoom() - 1);
	}
	else if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		if (CMinimap::GetInstance()->GetZoom() < 6)
			CMinimap::GetInstance()->SetZoom(CMinimap::GetInstance()->GetZoom() + 1);
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Hardware Abstraction
	theKeyboard->Read(dt);
	theMouse->Read(dt);

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo[currPlayer]->Update(dt);


	if (KeyboardController::GetInstance()->IsKeyReleased('B'))
	{
		currPlayer = Math::Min(++currPlayer, NUM_PLAYER - 1);
		ChangePlayer(currPlayer);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('V'))
	{
		currPlayer = Math::Max(--currPlayer, 0);
		ChangePlayer(currPlayer);
	}

	if (aChair->hooked)
	{				
		float speed = 0.05f;
		aChair->SetPos(Vector3(Lerpf(aChair->GetPos().x, playerInfo[currPlayer]->GetPos().x, speed), 0, Lerpf(aChair->GetPos().z, playerInfo[currPlayer]->GetPos().z, speed)));
		if ((aChair->GetPos() - playerInfo[currPlayer]->GetPos()).LengthSquared() < 4)
			aChair->SetIsDone(true);
	}
	else if (!aChair->IsDone())
	{
		if (EntityManager::GetInstance()->CheckOverlap(aChair->GetPos() + aChair->GetMinAABB(), aChair->GetPos() + aChair->GetMaxAABB(), playerInfo[currPlayer]->GetPos() + playerInfo[currPlayer]->GetMinAABB(), playerInfo[currPlayer]->GetPos() + playerInfo[currPlayer]->GetMaxAABB()))
		{
			playerInfo[currPlayer]->TakeDamage();
			playerInfo[currPlayer]->AddToMoveBy(playerInfo[currPlayer]->GetView() * -(aChair->GetPos() - playerInfo[currPlayer]->GetPos()).Length());
		}
	}
	for (int i = 0; i < NUM_ENEMY; ++i)
	{
		if (anEnemy3D[i]->IsDone())
			continue;
		if (anEnemy3D[i]->hooked)
		{
			float speed = 0.05f;
			anEnemy3D[i]->SetPos(Vector3(Lerpf(anEnemy3D[i]->GetPos().x, playerInfo[currPlayer]->GetPos().x, speed), 0, Lerpf(anEnemy3D[i]->GetPos().z, playerInfo[currPlayer]->GetPos().z, speed)));
			if ((anEnemy3D[i]->GetPos() - playerInfo[currPlayer]->GetPos()).LengthSquared() < 4)
				anEnemy3D[i]->SetIsDone(true);
		}		
		else if (EntityManager::GetInstance()->CheckOverlap(anEnemy3D[i]->GetPos() + anEnemy3D[i]->GetMinAABB(), anEnemy3D[i]->GetPos() + anEnemy3D[i]->GetMaxAABB(), playerInfo[currPlayer]->GetPos() + playerInfo[currPlayer]->GetMinAABB(), playerInfo[currPlayer]->GetPos() + playerInfo[currPlayer]->GetMaxAABB()))
		{
			playerInfo[currPlayer]->TakeDamage();
			playerInfo[currPlayer]->AddToMoveBy(playerInfo[currPlayer]->GetView() * -(anEnemy3D[i]->GetPos() - playerInfo[currPlayer]->GetPos()).Length());
		}
	}



	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	// Update NPC
	//enemyInfo->Update(dt);

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	DisplayText.precision(5);
	DisplayText.str("");
	DisplayText.clear();
	DisplayText << "FPS: " << CFPSCounter::GetInstance()->GetFrameRate();
	textObj[1]->SetText(DisplayText.str());

	//DisplayText.str("");
	//DisplayText.clear();
	//DisplayText << "P:" << currPlayer
	//			<< " W:" << playerInfo[currPlayer]->GetCurrentWeapon()
	//			<< " A:" << playerInfo[currPlayer]->GetCurrentWeaponAmmo() << "/" << playerInfo[currPlayer]->GetCurrentWeaponMaxAmmo()
	//			<< " C:" << playerInfo[currPlayer]->GetCurrentWeaponClips() << "/" << playerInfo[currPlayer]->GetCurrentWeaponMaxClips();
	//textObj[0]->SetText(DisplayText.str());

	DisplayText.str("");
	DisplayText.clear();
	DisplayText << "TIME:" << timeScale << " SCORE:" << score;
	textObj[0]->SetText(DisplayText.str());

	// Update camera effects
	theCameraEffects->Update(dt);
	//update minimap angle
	CMinimap::GetInstance()->SetAngle(atan2(playerInfo[currPlayer]->GetView().z, playerInfo[currPlayer]->GetView().x) * 57.2883513685549146);
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLineWidth(3);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	//cout << camera.GetCameraPos() << " / " << camera.GetCameraTarget() << " / " << camera.GetCameraUp() << endl;
	// PreRenderMesh
	RenderHelper::PreRenderMesh();

	EntityManager::GetInstance()->Render();

	//// Render the ammo dumps
	CAmmoDumpManager::GetInstance()->Render();

	// PostRenderMesh
	RenderHelper::PostRenderMesh();

	// Enable blend mode
	glEnable(GL_BLEND);

		// Setup 2D pipeline then render 2D
		int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
		int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
		GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -1000, 1000);
		GraphicsManager::GetInstance()->DetachCamera();

		// PreRenderMesh
		RenderHelper::PreRenderText(false,true,false,false);
			// Render Minimap
			playerInfo[currPlayer]->RenderHP();
			playerInfo[currPlayer]->RenderWeapInfo();
			CMinimap::GetInstance()->RenderUI();
		// PostRenderMesh
		RenderHelper::PostRenderText();

		// PreRenderMesh
		RenderHelper::PreRenderText();
		// Render player's weapons
		playerInfo[currPlayer]->RenderWeapon();
		// PostRenderMesh
		RenderHelper::PostRenderText();

		// Render Camera Effects
		if (theCameraEffects->GetBloodScreenStatus())
		{
			// PreRenderMesh
			RenderHelper::PreRenderText(false, true, false, false);
			theCameraEffects->RenderUI();
			// PostRenderMesh
			RenderHelper::PostRenderText();
		}

		// PreRenderMesh
		RenderHelper::PreRenderText();
		// Render the text objects		
		EntityManager::GetInstance()->RenderUI();
		// PostRenderMesh
		RenderHelper::PostRenderText();


	// Disable blend mode
	glDisable(GL_BLEND);
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	for (int i = 0; i < NUM_PLAYER; ++i)
	{
		playerInfo[i]->DetachCamera();
		delete playerInfo[i];
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];

	// Delete the ammo dumps
	CAmmoDumpManager::GetInstance()->Destroy();

	if (theCameraEffects)
	{
		delete theCameraEffects;
		theCameraEffects = NULL;
	}

	// Delete the minimap singleton
	CMinimap* theMinimap = CMinimap::GetInstance();
	theMinimap->Destroy();
	theMinimap = NULL;

	if (theMouse)
	{
		delete theMouse;
		theMouse = NULL;
	}
	if (theKeyboard)
	{
		delete theKeyboard;
		theKeyboard = NULL;
	}
}
