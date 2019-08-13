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
#include "DepthFBO.h"
#include "FloatFBO.h"

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

		MESH_PARTICLE_SMOKE,
		MESH_PARTICLE_RED,
		MESH_PARTICLE_BLUE,

		MESH_LIGHT_DEPTH,
		MESH_FLOAT_QUAD,
		MESH_BRIGHT_QUAD,
		MESH_BLUR_QUAD,
		MESH_FINAL_QUAD,

		NUM_MESH,
	};

	enum RENDER_PASS
	{
		P_DEPTH,
		P_HDR,
		P_FINAL,
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
	void RenderPlatform(MgrGraphics::SHADER shader);
	void RenderSkyPlane();
	void RenderTerrain(MgrGraphics::SHADER shader);

	//helpers
	void ShadowPass();
	void HdrPass();
	void BrightPass();
	void BlurPass();
	void FinalPass();	
	void RenderWorld(RENDER_PASS renderPass);

	void BindDeafultFBO();

private:	
	float lightMoveX;
	bool enableBloom;
	bool showFBO;
	
	CompMesh* m_meshList[NUM_MESH];
	std::vector<unsigned char> m_heightMap;
	ParticleList smoke, fireR, fireB;

	DepthFBO lightDepthFBO;
	FloatFBO hdrFBO;
	FloatFBO brightFBO;
	FloatFBO blurFBO[2];
};

#endif