#include "SceneShadow.h"
#include "Application.h"

SceneShadow::SceneShadow()
{
}

SceneShadow::~SceneShadow()
{
}

void SceneShadow::InitMeshes()
{	
	m_meshList[MESH_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair", 0, 1, 0);			   	

	m_meshList[MESH_PLATFORM] = MeshBuilder::GenerateQuad("platform", Color(0.6f, 0.6f, 1));
	//m_meshList[MESH_PLATFORM]->textureArray[0] = LoadTGA("Image//Acomg//platform.tga");	
	m_meshList[MESH_PLATFORM]->material.kShininess = 32.f;
	m_meshList[MESH_CUBE] = MeshBuilder::GenerateOBJ("cube blue", "OBJ//Acomg//Block.obj", Color(0.3f, 0, 1));
	m_meshList[MESH_CUBE]->textureArray[0] = LoadTGA("Image//Acomg//blockBlue.tga");
	
	m_meshList[MESH_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 1), 64, 64, 10);

	m_meshList[MESH_QUAD] = MeshBuilder::GenerateQuad("light depth", Color(1, 1, 1), 1);
	m_meshList[MESH_QUAD]->textureArray[0] = LoadTGA("Image//Acomg//particle.tga");

	m_meshList[MESH_LIGHT_DEPTH] = MeshBuilder::GenerateQuad("light depth", Color(1, 1, 1), 1);
	m_meshList[MESH_LIGHT_DEPTH]->textureArray[0] = lightDepthFBO.GetTexture();	
}

void SceneShadow::Init()
{
	SceneBase::Init();
	lightDepthFBO.Init(2048, 2048);
	InitMeshes();

	//define stuff in MgrGraphics
	MGR_GRAPHICS.DefineNumLights(1);
	MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_DIRECTIONAL, Position(0, 70, 30), Color(1, 1, 1, 1), 1.f);
	//MGR_GRAPHICS.DefineLightProps(1, true, Light::LIGHT_POINT, Position(0, 230, -10), Color(1, 1, 1, 1), 2.f);	
	//MGR_GRAPHICS.DefineFogProps(true, Fog::FOG_EXPO_SQ, Color(0.1f, 0.f, 0.f), 0.0005f);	
	MGR_GRAPHICS.DefineFogProps(false);	

	m_camera.Init(Vector3(0, 30, 50), Vector3(0, 30, 0), Vector3(0, 1, 0));


}

void SceneShadow::Update(double dt)
{	
	SceneBase::PreUpdate(dt);

	//Debugging options
	if (DEBUG_)
	{
		if (Application::IsKeyPressed(VK_F5))
			MGR_GRAPHICS.DefineNumLights(0);
		if (Application::IsKeyPressed(VK_F6))
			MGR_GRAPHICS.DefineNumLights(4);
		if (Application::IsKeyPressed(VK_F7))
			MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_POINT);
		else if (Application::IsKeyPressed(VK_F8))
			MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_DIRECTIONAL);
		else if (Application::IsKeyPressed(VK_F9))
			MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_SPOT);

		const static float LSPEED = 100.f;
		Vector3 newLightPos = MGR_GRAPHICS.GetLightPos(0);
		if (Application::IsKeyPressed('I'))
			newLightPos.z -= (float)(LSPEED * dt);
		if (Application::IsKeyPressed('K'))
			newLightPos.z += (float)(LSPEED * dt);
		if (Application::IsKeyPressed('J'))
			newLightPos.x -= (float)(LSPEED * dt);
		if (Application::IsKeyPressed('L'))
			newLightPos.x += (float)(LSPEED * dt);
		if (Application::IsKeyPressed('O'))
			newLightPos.y -= (float)(LSPEED * dt);
		if (Application::IsKeyPressed('U'))
			newLightPos.y += (float)(LSPEED * dt);

		MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_NULL, newLightPos);
	}

	//if (Application::IsKeyPressed(VK_LBUTTON))
	//	MGR_GRAPHICS.DefineBlendProps(true, Vector3(m_camera.position.x, 200, m_camera.position.z), 5, 2, 0);

	//MGR_GRAPHICS.SetUniformOffest(m_elapsedTime, m_elapsedTime * 0.25f);
	//MGR_GRAPHICS.SetUniformBlend(dt);	

	SceneBase::PostUpdate(dt);
}

void SceneShadow::Render()
{
	lightDepthFBO.BindForWriting();
		RenderFirstPass();	
	BindDeafultFBO();
	lightDepthFBO.BindForReading(GL_TEXTURE8);	
		RenderSecondPass();
}

void SceneShadow::Exit()
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		if (m_meshList[i])
			delete m_meshList[i];
	}

	SceneBase::Exit();
}

void SceneShadow::RenderFirstPass()
{
	SceneBase::PreRender();
	RenderWorld(true, MGR_GRAPHICS.DEPTH);
}

void SceneShadow::RenderSecondPass()
{	
	SceneBase::PreRender();
	RenderWorld();
	SceneBase::PostRender();
}

void SceneShadow::RenderWorld(bool forceShader, MgrGraphics::SHADER shader)
{
	if (!forceShader)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 10, 0);
		m_modelStack.Scale(100, 1, 100);
		m_modelStack.Rotate(-90, 1, 0, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_PLATFORM], MGR_GRAPHICS.LIT_BLEND, true);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(-30, 30, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_CUBE], MGR_GRAPHICS.LIT, true);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(30, 30, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_SPHERE], MGR_GRAPHICS.LIT, true);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 50, 0);
		m_modelStack.Scale(10, 10, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_LIGHT_DEPTH], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 30, 0);
		m_modelStack.Scale(10, 10, 1);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_QUAD], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();
	}
	else
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 10, 0);
		m_modelStack.Scale(100, 1, 100);
		m_modelStack.Rotate(-90, 1, 0, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_PLATFORM], shader);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(-30, 30, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_CUBE], shader);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(30, 30, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_SPHERE], shader);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 30, 0);
		m_modelStack.Scale(10, 10, 1);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_QUAD], shader);
		m_modelStack.PopMatrix();

	}
}

void SceneShadow::BindDeafultFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);	
}
