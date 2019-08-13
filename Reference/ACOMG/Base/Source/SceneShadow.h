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
		
		MESH_PLATFORM,
		MESH_CUBE,
		MESH_SPHERE,

		MESH_QUAD,
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
	void RenderWorld(bool forceShader = false, MgrGraphics::SHADER shader = MGR_GRAPHICS.LIT);

	void BindDeafultFBO();

private:	
	CompMesh* m_meshList[NUM_MESH];
	DepthFBO lightDepthFBO;

	int renderPass;
};

#endif