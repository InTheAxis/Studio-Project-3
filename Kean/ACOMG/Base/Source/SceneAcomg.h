#ifndef SCENE_ACOMG_H
#define SCENE_ACOMG_H

#include <vector>
#include <stack>
#include <sstream>

#include "SceneBase.h"
#include "UniLight.h"
#include "HelperShader.h"
#include "HelperResources.h"
#include "CompSpriteAnim.h"
#include "Particle.h"

class SceneAcomg : public SceneBase
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

		NUM_MESH,
	};
public:
	SceneAcomg();
	~SceneAcomg();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//helper
	void InitMeshes();	
	
	//helpers
	void RenderPlatform();
	void RenderSkyPlane(); 
	void RenderTerrain();	

private:	
	CompMesh* m_meshList[NUM_MESH];
	std::vector<unsigned char> m_heightMap;
	ParticleList particleList;
};

#endif