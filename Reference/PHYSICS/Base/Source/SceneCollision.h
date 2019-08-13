#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include <vector>
#include <map>

#include "SceneBase.h"
#include "GameObject.h"
#include "HelperPhysics.h"
#include "CompKinematicBody.h"
#include "CompTracker.h"
#include "MyMath.h"

class SceneCollision : public SceneBase
{
	enum GAMEOBJECT_ID
	{
		ID_BALL = 0,		
		ID_WALL,		
		ID_PILLAR,	
		ID_FLIPPER,	
		ID_BUMPER,	
		ID_PORTAL,
		ID_HOLE,		
		NUM_ID
	};

	unsigned POOL_SIZE[NUM_ID]
	{
		100,
		0,
		0,
		2,
		2,
	};	

	enum MESHES
	{
		MESH_SPHERE = 0,
		MESH_CUBE,
		MESH_CYLINDER,
		MESH_QUAD,
		NUM_MESHES,
	};
public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	GameObject* GOFetch(SceneCollision::GAMEOBJECT_ID id);
	GameObject* GOSpawn(SceneCollision::GAMEOBJECT_ID id);	
	void GORender(GameObject* go, MgrGraphics::SHADER shader = MGR_GRAPHICS.UNLIT, bool outline = false);
protected:
	//to override default view
	CameraBase m_camera;
	//height and width of the screen in world coords
	float m_worldWidth, m_worldHeight;
	
	int m_gameState;
	float m_middleWorld[2];

	float m_speed; //simul speed	
	Vector3 m_gravity;	

	float ballGrav = 2 ;

	int m_score1, m_score2;
	const int targetScore = 9;		

	double m_launchTime[2];

	double m_elapsedTime;
	double m_bounceScore;

	//pools
	std::map<GAMEOBJECT_ID, std::vector<GameObject*>> m_goList;	

	GameObject* m_ghost;
	GameObject* m_portal[2];
	GameObject* m_hole[2];
	GameObject* m_flipper[4];
	GameObject* m_pillar[6];

	//cache meshes
	std::vector<CompMesh> m_meshList;

	//Auditing
	float m_estimatedTime;

	void DebugControl(double dt);
	void FlipperControl(double dt);
	void BuildWorld();
	void CheckCollision(double dt);
	void CheckScore(double dtyy);
	void Launch(bool player);
};

#endif