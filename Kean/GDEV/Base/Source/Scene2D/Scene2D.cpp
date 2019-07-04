#include "Scene2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "../Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "../EntityManager.h"

#include "../GenericEntity.h"
#include "../TextEntity.h"
#include "../SpriteEntity.h"
#include "../Light.h"
#include "Strategy_Kill.h"

#include "FPSCounter.h"
#include "RenderHelper.h"

#include <iostream>
using namespace std;

CScene2D* CScene2D::sInstance = new CScene2D(SceneManager::GetInstance());

CScene2D::CScene2D()
	: thePlayerInfo(NULL)
	, m_iNumEnemy(3)
{
}

CScene2D::CScene2D(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Scene2D", this);
}

CScene2D::~CScene2D()
{
}

void CScene2D::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default",
															"Shader//comg.vertexshader",
															"Shader//comg.fragmentshader");

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

	glDisable(GL_DEPTH_TEST);
	// Enable blend mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("jump", "Image//Mario-jump-sound.mp3");

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	// Load background image
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_BKGROUND")->textureID = LoadTGA("Image//Scene2D_Background.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_GROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_GROUND")->textureID = LoadTGA("Image//Scene2D_Tile_Ground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_WALL", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_WALL")->textureID = LoadTGA("Image//Scene2D_Tile_Wall.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO")->textureID = LoadTGA("Image//Scene2D_Tile_Hero.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_TREE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_TREE")->textureID = LoadTGA("Image//Scene2D_Tile_Ladder.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_REARSTRUCTURE", Color(1, 1, 1), 1.f);	
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_REARSTRUCTURE")->textureID = 
		LoadTGA("Image//Scene2D_RearStructure.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_0", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_0")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_0.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_1")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_2")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_3")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_3.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_LEFT_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_LEFT_1")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Left_1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_LEFT_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_LEFT_2")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Left_2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_LEFT_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_LEFT_3")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Left_3.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_ENEMY", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_ENEMY")->textureID = LoadTGA("Image//Scene2D_Tile_Enemy.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_TREASURECHEST", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_TREASURECHEST")->textureID = LoadTGA("Image//coin.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_PLAY_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_PLAY_1")->textureID = LoadTGA("Image//play1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_PLAY_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_PLAY_2")->textureID = LoadTGA("Image//play2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_PLAY_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_PLAY_3")->textureID = LoadTGA("Image//play3.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_CLOUD", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_CLOUD")->textureID = LoadTGA("Image//cloud.tga");
	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	// Create the playerinfo instance, which manages all information about the player
	thePlayerInfo = CPlayerInfo2D::GetInstance();
	thePlayerInfo->Init();

	// Initialise and load the tile map

	CMap* frontMap, *rearMap;
	
	frontMap = new CMap();
	frontMap->Init(600, 800, 24, 32, 600, 800);
	frontMap->LoadMap("Image//MapDesign0.csv");

	rearMap = new CMap();
	rearMap->Init(600, 800, 24, 32, 600, 800);
	rearMap->LoadMap("Image//RearMapDesign0.csv");

	m_cMaps[0] = std::tie(frontMap, rearMap);

	frontMap = new CMap();
	frontMap->Init(600, 800, 24, 32, 600, 1600);
	frontMap->LoadMap("Image//MapDesign1.csv");

	rearMap = new CMap();
	rearMap->Init(600, 800, 24, 32, 600, 1600);
	rearMap->LoadMap("Image//RearMapDesign1.csv");

	m_cMaps[1] = std::tie(frontMap, rearMap);



	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	// Create the background image
	Scene2D_Background = Create::Sprite2DObject("SCENE2D_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f), true);
	Scene2D_TileGround = Create::Sprite2DObject("SCENE2D_TILE_GROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_TileWall = Create::Sprite2DObject("SCENE2D_TILE_WALL",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero = Create::Sprite2DObject("SCENE2D_TILE_HERO",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_TileTree = Create::Sprite2DObject("SCENE2D_TILE_TREE",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_RearStructure = Create::Sprite2DObject("SCENE2D_TILE_REARSTRUCTURE",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	Scene2D_Hero_Animated = new SpriteEntity*[7];
	Scene2D_Hero_Animated[0] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_0",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[1] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[2] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[3] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[4] = Create::Sprite2DObject("SCENE2D_TILE_HERO_LEFT_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[5] = Create::Sprite2DObject("SCENE2D_TILE_HERO_LEFT_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[6] = Create::Sprite2DObject("SCENE2D_TILE_HERO_LEFT_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	Scene2D_Goodies_TreasureChest = Create::Sprite2DObject("SCENE2D_TILE_TREASURECHEST",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	Scene2D_Play[0] = Create::Sprite2DObject("SCENE2D_TILE_PLAY_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Play[1] = Create::Sprite2DObject("SCENE2D_TILE_PLAY_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Play[2] = Create::Sprite2DObject("SCENE2D_TILE_PLAY_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	Scene2D_Cloud = Create::Sprite2DObject("SCENE2D_CLOUD",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	
	// Set the Animation indices
	thePlayerInfo->SetRightIndices(0, 3);
	thePlayerInfo->SetLeftIndices(4, 6);

	// Create the 3 enemies
	m_iNumEnemy = 3;
	theEnemy = new CEnemy*[m_iNumEnemy];
	for (int i = 0; i < m_iNumEnemy; i++)
	{
		theEnemy[i] = new CEnemy();
		theEnemy[i]->Init(frontMap);
		theEnemy[i]->ChangeStrategy(new CStrategy_Kill(), false);
		theEnemy[i]->SetPlayerInfo(thePlayerInfo);
	}
	Scene2D_Enemy = Create::Sprite2DObject("SCENE2D_TILE_ENEMY",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	// Create a text object
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", 
										  Vector3(5, 5 + fontSize*i + halfFontSize, 0.0f), 
										  "", 
										  Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
	}
	textObj[0]->SetText("<First Row>");
	textObj[1]->SetText("<Second Row>");
	textObj[2]->SetText("<Third Row>");

	coins = 0;
	currentLevel = 999;
	ChangeMap(0);
}

void CScene2D::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (KeyboardController::GetInstance()->IsKeyDown('5'))
		ChangeMap(1);
	if (KeyboardController::GetInstance()->IsKeyDown('6'))
		ChangeMap(0);
	
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

	// Update the thePlayerInfo depending on map
	if (currentLevel == 0)
	{
		if (KeyboardController::GetInstance()->IsKeyDown('W'))
			thePlayerInfo->MoveUpDown(true, 1.0f);
		else if (KeyboardController::GetInstance()->IsKeyDown('S'))
			thePlayerInfo->MoveUpDown(false, 1.0f);
		
		thePlayerInfo->UpdateWrapAround();
	}
	else if (currentLevel == 1)
	{
		if (KeyboardController::GetInstance()->IsKeyDown('W'))
			thePlayerInfo->UpdateClimbLadder(true);		
		if (KeyboardController::GetInstance()->IsKeyDown('S'))
			thePlayerInfo->UpdateClimbLadder(false);

		// If the user presses SPACEBAR, then make him jump		
		if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
		{
			if (!thePlayerInfo->m_jumpedMax)
			{
				thePlayerInfo->SetToJumpUpwards(!thePlayerInfo->isOnAir());
				CSoundEngine::GetInstance()->PlayASound("jump");
			}
			else
			{
				if(!thePlayerInfo->isOnAir())
					thePlayerInfo->m_jumpedMax = false;
			}
		}
		else		
			thePlayerInfo->SetOnFreeFall(thePlayerInfo->isOnAir());		
	
	}
	thePlayerInfo->Update(dt);

	// Update the enemies
	for (int i = 0; i < m_iNumEnemy; ++i)
	{
		// Update the enemy
		theEnemy[i]->Update();

		if ((theEnemy[i]->GetPos() - thePlayerInfo->GetPos()).LengthSquared() < m_currentMap->GetTileSize_Height() * m_currentMap->GetTileSize_Width())
			thePlayerInfo->die = true;
	}

	GraphicsManager::GetInstance()->UpdateLights(dt);

	UpdateGoodies(currentLevel, dt);

	if (thePlayerInfo->die)
		ChangeMap(currentLevel);
}

void CScene2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	//EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(),
															  0, Application::GetInstance().GetWindowHeight(),
															  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// PreRenderMesh
	RenderHelper::PreRenderMesh();
		// Render the required entities
		EntityManager::GetInstance()->RenderUI();

		// Render the rear tile map
		RenderRearTileMap();
		// Render the tile map
		RenderTileMap();
		// Render the Enemy
		RenderEnemy();
		// Render the player
		RenderPlayer();
	// PostRenderMesh
	RenderHelper::PostRenderMesh();

	// PreRenderMesh
	RenderHelper::PreRenderText();
		textObj[0]->RenderUI();
		textObj[1]->RenderUI();
		textObj[2]->RenderUI();
	// PostRenderMesh
	RenderHelper::PostRenderText();

}

void CScene2D::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

	// Delete the sprites
	delete Scene2D_Goodies_TreasureChest;
	Scene2D_Goodies_TreasureChest = NULL;
	delete Scene2D_Enemy;
	Scene2D_Enemy = NULL;
	delete Scene2D_Background;
	Scene2D_Background = NULL;
	delete Scene2D_TileGround;
	Scene2D_TileGround = NULL;
	delete Scene2D_Hero;
	Scene2D_Hero = NULL;
	delete Scene2D_TileTree;
	Scene2D_TileTree = NULL;
	delete Scene2D_RearStructure;
	Scene2D_RearStructure = NULL;

	for (int i = 0; i < m_iNumEnemy; ++i)
	{
		delete theEnemy[i];
		theEnemy[i] = NULL;
	}
	delete theEnemy;
	theEnemy = NULL;

	for (int i = 0; i < 7; ++i)
	{
		delete Scene2D_Hero_Animated[i];
		Scene2D_Hero_Animated[i] = NULL;
	}
	delete Scene2D_Hero_Animated;
	Scene2D_Hero_Animated = NULL;

	if (thePlayerInfo->DropInstance() == false)
	{
		cout << "CScene2D: Unable to drop CPlayerInfo2D class" << endl;
	}

	//TODO delete map
	CMap* temp1, *temp2;
	std::tie(temp1, temp2) = m_cMaps[0];
	delete temp1;
	delete temp2;
	std::tie(temp1, temp2) = m_cMaps[1];
	delete temp1;
	delete temp2;
}

void CScene2D::RenderTileMap()
{
	int m = 0;
	thePlayerInfo->UpdateMapFineOffset();	

	for (int i = 0; i < m_currentMap->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < m_currentMap->GetNumOfTiles_Width()+1; k++)
		{
			m = thePlayerInfo->GetTileOffset_x() + k;

			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_currentMap->getNumOfTiles_MapWidth())
				break;

			if (m_currentMap->theScreenMap[i][m] == 1)
			{
				Scene2D_TileWall->SetPosition(Vector3(k*m_currentMap->GetTileSize_Width() + kiHalfTileWidth
															- thePlayerInfo->GetMapFineOffset_x(), 
														575 - i* m_currentMap->GetTileSize_Height() + kiHalfTileHeight,
														0.0f));
				Scene2D_TileWall->RenderUI();
			}
			else if (m_currentMap->theScreenMap[i][m] == 2)
			{
				Scene2D_TileGround->SetPosition(Vector3(k*m_currentMap->GetTileSize_Width() + kiHalfTileWidth
															- thePlayerInfo->GetMapFineOffset_x(),
													  575 - i* m_currentMap->GetTileSize_Height() + kiHalfTileHeight,
													  0.0f));
				Scene2D_TileGround->RenderUI();
			}
			else if (m_currentMap->theScreenMap[i][m] == 3)
			{
				Scene2D_TileTree->SetPosition(Vector3(k*m_currentMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetMapFineOffset_x(),
					575 - i * m_currentMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_TileTree->RenderUI();
			}
			
			CGoodies* temp = CGoodiesFactory::GetInstance()->GetGoodies(m, i);

			if (temp && temp->GetStatus())
			{
				if (m_currentMap->theScreenMap[i][m] == 100 || m_currentMap->theScreenMap[i][m] == 101)
				{
					//render nth
				}
				else if (m_currentMap->theScreenMap[i][m] == 200)
				{
					Scene2D_Goodies_TreasureChest->SetPosition(Vector3(k*m_currentMap->GetTileSize_Width() + kiHalfTileWidth
						- thePlayerInfo->GetMapFineOffset_x(),
						575 - i * m_currentMap->GetTileSize_Height() + kiHalfTileHeight,
						0.0f));
					Scene2D_Goodies_TreasureChest->RenderUI();
				}
			}
		}
	}

	DisplayText.precision(5);
	DisplayText.str("");
	DisplayText.clear();
	DisplayText << "Coins: " << coins << endl;
	textObj[1]->SetText(DisplayText.str());

	DisplayText.str("");
	DisplayText.clear();
	DisplayText << "mapOffset_x: " << thePlayerInfo->mapOffset_x << endl;
	textObj[0]->SetText(DisplayText.str());

	//DisplayText.str("");
	//DisplayText.clear();
	//DisplayText << "Enemies: ";
	//for (int i = 0; i < m_iNumEnemy; ++i)
	//{
	//	DisplayText << theEnemy[i]->GetPos_x() << "(" << theEnemy[i]->GetPos_x() - thePlayerInfo->mapOffset_x << ")" << ", ";
	//}
	//textObj[2]->SetText(DisplayText.str());
}

// Render the rear tile map
void CScene2D::RenderRearTileMap(void)
{
	int m = 0;
	thePlayerInfo->UpdateRearMap();

	for (int i = 0; i < m_currentRearMap->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < m_currentRearMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = thePlayerInfo->GetRearTileOffset_x() + k;

			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_currentRearMap->getNumOfTiles_MapWidth())
				break;

			if (m_currentRearMap->theScreenMap[i][m] == 1)
			{
				Scene2D_Play[0]->SetPosition(Vector3(k*m_currentRearMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetRearMapFineOffset_x(),
					575 - i* m_currentRearMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_Play[0]->RenderUI();
			}
			else if (m_currentRearMap->theScreenMap[i][m] == 2)
			{
				Scene2D_Play[1]->SetPosition(Vector3(k*m_currentRearMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetRearMapFineOffset_x(),
					575 - i* m_currentRearMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_Play[1]->RenderUI();
			}
			else if (m_currentRearMap->theScreenMap[i][m] == 3)
			{
				Scene2D_Play[2]->SetPosition(Vector3(k*m_currentRearMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetRearMapFineOffset_x(),
					575 - i * m_currentRearMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_Play[2]->RenderUI();
			}
			else if (m_currentRearMap->theScreenMap[i][m] == 4)
			{
				Scene2D_Cloud->SetPosition(Vector3(k*m_currentRearMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetRearMapFineOffset_x(),
					575 - i * m_currentRearMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_Cloud->RenderUI();
			}
		}
	}
}

void CScene2D::RenderPlayer()
{
	//	Scene2D_Hero->SetPosition(thePlayerInfo->GetPos());
	//	Scene2D_Hero->RenderUI();

	// Display the player
	Scene2D_Hero_Animated[thePlayerInfo->GetAnimationIndex()]->SetPosition(thePlayerInfo->GetPos());
	Scene2D_Hero_Animated[thePlayerInfo->GetAnimationIndex()]->RenderUI();
}

// Render the enemy
void CScene2D::RenderEnemy(void)
{
	if (currentLevel == 1)
	{
		// Render the enemies
		for (int i = 0; i < m_iNumEnemy; ++i)
		{
			int theEnemy_x = theEnemy[i]->GetPos_x() - thePlayerInfo->mapOffset_x;
			int theEnemy_y = theEnemy[i]->GetPos_y();

			Scene2D_Enemy->SetPosition(Vector3(theEnemy_x, theEnemy_y, 0));
			Scene2D_Enemy->RenderUI();
		}
	}
}

void CScene2D::ChangeMap(unsigned levelId)
{

	if (m_cMaps.count(levelId) <= 0)
		return;

	CMap *temp1, *temp2;

	std::tie(temp1, temp2) = m_cMaps[levelId];

	if (currentLevel == levelId)
		std::cout << "Reloading Map\n";	
	thePlayerInfo->die = false;

	//else
	currentLevel = levelId;
	m_currentMap = temp1;
	m_currentRearMap = temp2;

	switch (levelId)
	{
	case 0:
		thePlayerInfo->SetPos(Vector3(375.f + kiHalfTileWidth, 125.0f + kiHalfTileHeight));
		thePlayerInfo->SetBoundary(Vector3(675.0f, 575.0f, 0.0f), Vector3(100.0f, 25.0f, 0.0f));
	
		CreateGoodies(levelId, GID_CHANGEMAP);
		break;
	case 1:	
		thePlayerInfo->SetPos(Vector3(75.0f + kiHalfTileWidth, 125.0f + kiHalfTileHeight));
		thePlayerInfo->SetBoundary(Vector3(675.0f, 575.0f, 0.0f), Vector3(100.0f, 25.0f, 0.0f));
		
		CreateGoodies(levelId, GID_TREASURECHEST);
		break;
	}
	thePlayerInfo->SetTileSize(m_currentMap->GetTileSize_Width(), m_currentMap->GetTileSize_Height());
	thePlayerInfo->SetMap(m_currentMap);
	thePlayerInfo->SetRearMap(m_currentRearMap);
	thePlayerInfo->mapOffset_x = 0;
	thePlayerInfo->mapOffset_y = 0;

	theEnemy[0]->SetPos(450 + 12, 100 + 12);
	theEnemy[1]->SetPos(500 + 12, 100 + 12);
	theEnemy[2]->SetPos(550 + 12, 100 + 12);
}

// Create the Goodies
void CScene2D::CreateGoodies(unsigned levelId, GoodiesID goodieId)
{
	if (m_cMaps.count(levelId) <= 0)
		return;

	CMap* frontMap = std::get<0>(m_cMaps[levelId]);

	for (int i = 0; i < frontMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < frontMap->getNumOfTiles_MapWidth(); k++)
		{
			if (frontMap->theScreenMap[i][k] == goodieId)
			{
				(CGoodiesFactory::GetInstance()->Create(goodieId, k, i))->SetStatus(true);
			}
			else
			{
				CGoodiesFactory::GetInstance()->RemoveGoodies(k, i);
			}
		}
	}
}

void CScene2D::UpdateGoodies(unsigned levelId, double dt)
{	
	CGoodies* goodie = thePlayerInfo->CollideWithGoodies();
	if (!goodie || !goodie->GetStatus())
		return;

	switch (goodie->GetID())
	{
	case GID_CHANGEMAP:		
		ChangeMap(levelId == 0);
		break;
	case GID_TREASURECHEST:
		goodie->SetStatus(false);
		coins++;
		break;
	}
}
