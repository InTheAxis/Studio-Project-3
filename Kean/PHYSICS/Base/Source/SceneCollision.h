#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include <vector>
#include <map>

#include "SceneBase.h"
#include "GameObject.h"
#include "HelperPhysics.h"
#include "CompKinematicBody.h"

class SceneCollision : public SceneBase
{
	enum GAMEOBJECT_ID
	{
		ID_BALL = 0,		
		ID_WALL,		
		ID_PILLAR,		
		NUM_ID
	};

	unsigned POOL_SIZE[NUM_ID]
	{
		100,
	};	

	enum MESHES
	{
		MESH_SPHERE = 0,
		MESH_CUBE,
		MESH_CYLINDER,
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
	void CheckCollision(double dt);
protected:
	//to override default view
	CameraBase m_camera;
	//height and width of the screen in world coords
	float m_worldWidth, m_worldHeight;
	
	float m_speed; //simul speed	

	double m_elapsedTime;
	double m_bounceInput;

	//pools
	std::map<GAMEOBJECT_ID, std::vector<GameObject*>> m_goList;	

	GameObject* m_ghost;

	//cache meshes
	std::vector<CompMesh> m_meshList;

	//Auditing
	bool m_timerStarted;
	float m_timeTaken1, m_estimatedTime;
};

#endif