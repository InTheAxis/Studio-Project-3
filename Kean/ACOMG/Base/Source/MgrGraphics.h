#ifndef MGR_GRAPHICS_H
#define MGR_GRAPHICS_H

#include "Singleton.h"
#include "HelperShader.h"
#include "CameraBase.h"
#include "MatrixStack.h"
#include "UniLight.h"
#include "UniMaterial.h"
#include "UniFog.h"
#include "UniBlend.h"
#include "CompMesh.h"
#include "CompSpriteAnim.h"
#include "Particle.h"

//macro to not need to keep giving the current shader
#define Uniform_(uni, data) HelperShader::SetUniform(m_shaders[m_currentShader], uni, data)

#define MGR_GRAPHICS MgrGraphics::Instance()

class MgrGraphics : public Singleton<MgrGraphics>
{
public:
	friend class Singleton<MgrGraphics>;
	virtual void Init();
	virtual void Drop();

	enum SHADER
	{
		LIT = 0,
		UNLIT,
		UNLIT_TINT,
		SIMPLE,
		LIT_BLEND,
		WAVE_UNLIT_SCROLL,
		BILLBOARD_UNLIT,
		FIRST_PASS,
		NUM_SHADERS
	};

	//change shaders
	void UseShader(SHADER program = LIT);

	//store relevant references needed to render
	void AttachCamera(CameraBase* cam);
	void AttachModelStack(MS* modelStack);
	
	//opengl clear etc
	void PreRenderSetUp();

	//setting of uniforms
	void SetUniformsMVP();
	void SetUniformsLight();
	void SetUniformsFog();
	void SetUniformsMesh(const CompMesh* mesh);
	void SetUniformOffest(float vertOffset, float texOffset);
	void SetUniformBlend(double dt);

	//render api
	void RenderMesh(CompMesh *mesh, SHADER shader, bool outline = false);
	void RenderSpriteAnim(CompMesh *mesh, SHADER shader, bool outline = false);	
	void RenderMeshOnScreen(CompMesh *mesh, SHADER shader, float size = 1.0f, float x = 0.0f, float y = 0.0f);
	void RenderText(CompMesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(CompMesh* mesh, std::string text, Color color, float size, float x, float y);

	//define a custom Projection
	void DefineCustomProjection(Mtx44& proj);
	//define num lights to use
	void DefineNumLights(int zeroToEight);
	//define light properties, put NULL or 0 if keep status quo
	void DefineLightProps(int index, bool enabled, Light::LIGHT_TYPE type = Light::LIGHT_NULL, Position pos = NULL, Color color = NULL, float power = NULL); //add more as needed?
	//define fog properties, put NULL or 0 if keep status quo
	void DefineFogProps(bool enabled, Fog::FOG_TYPE type = Fog::FOG_NULL, Vector4 color = NULL, float density = NULL, float start = NULL, float end = NULL);
	//define blend properties
	void DefineBlendProps(bool enabled, Vector3 pos, float radius, float dura, float cutoff = -1);

	//getters
	int GetNumLightsDefined();
	Vector3 GetLightPos(int index);
private:
	MgrGraphics::MgrGraphics() {}
	MgrGraphics::~MgrGraphics() {}

	//shaders
	SHADER m_currentShader;
	unsigned m_shaders[NUM_SHADERS];

	//mvp
	CameraBase* m_cameraPtr;
	MS* m_modelStackPtr;
	MS m_viewStack, m_projStack;

	//projections cache, because they are unlikely to change
	Mtx44 m_defaultProj; //default
	Mtx44 m_customProj; //to store user-defined proj

	//lights
	static const int MAX_LIGHTS = 8;
	Light m_lights[MAX_LIGHTS];
	int m_numLights; //num to actually use

	//fog
	Fog m_fog;

	//for blend
	Blend m_blend;

	//helpers
	void DrawMesh(CompMesh* mesh);
	void DrawMesh(CompMesh* mesh, unsigned count, unsigned offset);
	void DrawSpriteAnim(CompSpriteAnim* anim);
};


#endif 
