#ifndef SCENE_PHYS_H
#define SCENE_PHYS_H

#include <map>
#include <vector>
#include <sstream>

#include "SceneBase.h"
#include "UniLight.h"
#include "HelperShader.h"
#include "HelperResources.h"
#include "HelperPhysics.h"

//custom components
#include "CompKinematicBody.h"
#include "CompHealth.h"
#include "CompTimer.h"
#include "CompTracker.h"
#include "CompGravitation.h"
#include "CompAmmo.h"
#include "CompPowerup.h"

class ScenePhysics : public SceneBase
{
	//constants
	static const int MAX_LIVES = 3;
	
	//enums
	enum GAMEOBJECT_ID
	{
		GENERIC = 0,
		GO_BG,
		GO_SHIP,
		GO_ASTEROID,

		GO_ENEMY_KAMIKAZE,
		GO_ENEMY_SHOOTING,

		GO_LMG_ENEMY,
		GO_LMG_BULLET,
		GO_MISSLE,
		
		GO_BLACKHOLE,
		GO_REPULSOR,

		GO_BUDDY_HEAL,
		GO_BUDDY_KAMIKAZE,

		GO_POWERUP,

		GO_TOTAL,
	};
	unsigned POOL_SIZE[GO_TOTAL] =
	{
		0,
		1,
		1,
		100,

		//enemy
		40,
		40,

		//shooting
		200,
		100,
		10,

		//blackhole, repulsor
		20,
		20,

		//buddy
		2,
		2,

		//powerup
		20,
	};	

	enum MISC_MESHES
	{
		MM_HPBAR = 0,
		MM_TARGET,

		MM_MENU,
		MM_LOSE,

		MM_TOTAL,
	};

	enum BOUNCE_TIMES
	{
		BOUNCE_INPUT = 0,
		BOUNCE_COLLISION,
		BOUNCE_SPAWN_AST,
		BOUNCE_SPAWN_ENEMY,
		BOUNCE_SPAWN_BLACKHOLE,
		BOUNCE_SPAWN_POWERUP,

		BOUNCE_TOTAL,
	};
	double BOUNCE_DELAY[BOUNCE_TOTAL] = 
	{
		0.125,
		0.005,
		5,
		6,
		10,
		3,
	};

	enum AMMO_TYPE
	{
		AMMO_LMG = 0,
		AMMO_MISSLE,
	};

	enum GAME_STATE
	{
		GS_MENU,
		GS_GAME,
		GS_LOSE,
	};
public:
	ScenePhysics();
	~ScenePhysics();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	std::map<GAMEOBJECT_ID, std::vector<GameObject*>> m_goList;	
	
	//to cache same meshes
	CompMesh* meshList[GO_TOTAL];

	//misc meshes
	CompMesh* miscMeshList[MM_TOTAL];

	//for convenience
	GameObject* player;	

	//to override default view
	CameraBase m_camera;

	//height and width of the screen in world coords
	float m_worldWidth, m_worldHeight;
	
	//stores bounce times for scene bouced updates
	double m_bounceTimes[BOUNCE_TOTAL];

	//game variables
	int m_lives;
	int m_score;
	int m_numEnemies;	
	GAME_STATE gameState;

	//init stuff
	void Reset();
	void InitMiscMeshes();
	void GOInit();
	GameObject* GOSpawn(GAMEOBJECT_ID id);
	GameObject* GOFetch(GAMEOBJECT_ID id);
	//render
	void GORenderPool(GAMEOBJECT_ID id, MgrGraphics::SHADER shader = MGR_GRAPHICS.UNLIT, bool outline = false);
	//update
	void BouncedUpdate(double dt);
	void QuickUpdate(double dt); //non bounced	
	void GOUpdate(GAMEOBJECT_ID id, double dt);
	void GOCheckCollision(GAMEOBJECT_ID id);
	
	//helpers
	void PlayerController(double dt);
	void PlayerReset();	
};

#endif