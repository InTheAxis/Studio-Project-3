#ifndef SCENE_SHADOW_H
#define SCENE_SHADOW_H

#include <vector>
#include <stack>
#include <sstream>

#include "SceneBase.h"
#include "UniLight.h"
#include "HelperShader.h"
#include "HelperResources.h"
#include "CompSpriteAnim.h"
#include "Particle.h"
#include "DepthFBO.h"

class SceneShadow : public SceneBase
{
	enum MESH
	{
		MESH_CROSSHAIR,
		
		MESH_SKYPLANE,
		MESH_TERRAIN,

		MESH_PLATFORM,
		MESH_CUBE_R,
		MESH_CUBE_B,

		MESH_TITLE,
		MESH_STAGE,

		MESH_BLADE_R,
		MESH_BLADE_B,

		MESH_WATER,
		MESH_BILLBOARD,

		MESH_PARTICLE_WATER,

		MESH_LIGHT_DEPTH,

		NUM_MESH,
	};
public:
	SceneShadow();
	~SceneShadow();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//helper
	void InitMeshes();	
	
	//helpers
	void RenderFirstPass();
	void RenderSecondPass();
	void RenderWorld();
	void RenderPlatform();
	void RenderSkyPlane(); 
	void RenderTerrain();	

private:	
	CompMesh* m_meshList[NUM_MESH];
	std::vector<unsigned char> m_heightMap;
	ParticleList particleList;

	DepthFBO lightDepthFBO;

	int renderPass;
};

#endif