#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include <map>
#include <tuple>

#include "Scene.h"
#include "Mtx44.h"
#include "../FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../GenericEntity.h"
#include "../SpriteEntity.h"
#include "Map.h"
#include "PlayerInfo2D.h"
#include "Enemy.h"

#include "GoodiesFactory.h"
#include "../SoundEngine.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class CScene2D : public Scene
{	
public:
	CScene2D();
	~CScene2D();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	CScene2D(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	FPSCamera camera;
	ostringstream DisplayText;
	TextEntity* textObj[3];

	static CScene2D* sInstance; // The pointer to the object that gets registered

	// Tile information
	const int kiHalfTileWidth = 12;
	const int kiHalfTileHeight = 12;

	// Scene2D images
	SpriteEntity* Scene2D_Background;
	SpriteEntity* Scene2D_Hero;
	SpriteEntity* Scene2D_TileGround;
	SpriteEntity* Scene2D_TileWall;
	SpriteEntity* Scene2D_TileTree;
	SpriteEntity* Scene2D_RearStructure;
	SpriteEntity** Scene2D_Hero_Animated;
	SpriteEntity* Scene2D_Goodies_TreasureChest;
	SpriteEntity* Scene2D_Play[3];
	SpriteEntity* Scene2D_Cloud;
	// Handle to the tilemaps
	CMap* m_currentMap;
	CMap* m_currentRearMap;	// Pointer to the rear map

	unsigned currentLevel;
	int coins;

	std::map<unsigned, std::tuple<CMap*, CMap*>> m_cMaps; //handle to all tilemaps based on level id

	// This class stores all information about the player / avatar
	CPlayerInfo2D* thePlayerInfo;

	// The enemies
	CEnemy** theEnemy;
	int m_iNumEnemy;
	SpriteEntity* Scene2D_Enemy;

	// Render the tile map
	void RenderTileMap(void);
	// Render the rear tile map
	void RenderRearTileMap(void);
	// Render the player
	void RenderPlayer(void);
	// Render the enemy
	void RenderEnemy(void);

	//change maps
	void ChangeMap(unsigned levelId);
	// Create the Goodies
	void CreateGoodies(unsigned levelId, GoodiesID goodieId);
	//update the goodies
	void UpdateGoodies(unsigned levelId, double dt);
};

#endif