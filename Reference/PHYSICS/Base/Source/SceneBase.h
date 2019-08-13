#ifndef SCENE_H
#define SCENE_H

#include "GL\glew.h"
#include "CameraFPS.h"
#include "MatrixStack.h"
#include "Mtx44.h"
#include "MeshBuilder.h"
#include "HelperShader.h"
#include "HelperResources.h"
#include "UniLight.h"
#include "GameObject.h"
#include "MgrGraphics.h"
//essential components
#include "CompMesh.h"
#include "CompTransform.h"

using HelperResources::LoadTGA;

//#ifdef _DEBUG
//	#define DEBUG_ m_debug
//#else
//	#define DEBUG_ 0
//#endif

#define DEBUG_ m_debug
//#define DEBUG_ 0

class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	//call these virtual functions in derived virtual funcs!!!
	virtual void Init();
	virtual void Update(double dt) = 0; //call pre and post below instead
	virtual void Render() = 0; //call pre and post below instead
	virtual void Exit();

protected:
	//default fps camera
	CameraFPS m_camera;

	//modelstack for hierachial modelling
	MS m_modelStack;
	
	//counters
	double m_fps, m_elapsedTime;
	
	//flags
	bool m_debug;

	//helpers for derived scenes
	void PreUpdate(double dt);
	void PostUpdate(double dt);
	void PreRender();
	void PostRender();
	
	//for convenience, calls MgrGraphics equivalent
	//render a GO's mesh component
	void RenderMeshGO(GameObject* go, MgrGraphics::SHADER shader, bool outline = false);
	//render text on screen, using TEXt mesh above
	void RenderText(std::string text = "HELLO WORLD!", Color color = Color(0, 0, 0));
	void RenderTextOnScreen(std::string text = "HELLO WORLD!", float x = 1.f, float y = 1.f, Color color = Color(0, 0, 0), float size = 3.f);

private:
	//common meshes to draw
	CompMesh *AXES, *TEXT, *LIGHTBALL;

	//helpers for this scene
	void RenderLights();
};

#endif