#include "SceneBase.h"
#include "Application.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	delete AXES;
	delete LIGHTBALL;
	delete TEXT;
}

void SceneBase::Init()
{
	MGR_GRAPHICS.Init();

	m_camera.Init(Vector3(0, 220, 10), Vector3(0, 220, -1), Vector3(0, 1, 0));
	MGR_GRAPHICS.AttachCamera(&m_camera);
	m_modelStack.LoadIdentity();
	MGR_GRAPHICS.AttachModelStack(&m_modelStack);
	
	AXES = MeshBuilder::GenerateAxes("reference");

	TEXT = MeshBuilder::GenerateText("text", 16, 16);
	TEXT->textureArray[0] = LoadTGA("Image//lazyfont.tga");
	TEXT->material.kAmbient.Set(1, 0, 0);

	LIGHTBALL = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);

	m_elapsedTime = 0.0;
	m_fps = 1000.0;
	m_debug = true;
}

void SceneBase::PreUpdate(double dt)
{
	if (DEBUG_)
	{
		if (Application::IsKeyPressed(VK_F1))
			glEnable(GL_CULL_FACE);
		if (Application::IsKeyPressed(VK_F2))
			glDisable(GL_CULL_FACE);
		if (Application::IsKeyPressed(VK_F3))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (Application::IsKeyPressed(VK_F4))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (Application::IsKeyPressed(VK_F5))
			MgrGraphics::Instance().UseShader(MgrGraphics::LIT);
		else if (Application::IsKeyPressed(VK_F6))
			MgrGraphics::Instance().UseShader(MgrGraphics::UNLIT);
	}
	if (Application::IsKeyPressed('1'))
		m_debug = true;
	else if (Application::IsKeyPressed('0'))
		m_debug = false;

	m_elapsedTime += dt;
}

void SceneBase::PostUpdate(double dt)
{
	m_fps = (1.0 / dt);

	m_camera.Update(dt);
}

void SceneBase::PreRender()
{
	MgrGraphics::Instance().PreRenderSetUp();

	if (DEBUG_)
	{
		glLineWidth(5);
		MGR_GRAPHICS.RenderMesh(AXES, MGR_GRAPHICS.UNLIT);
		RenderLights();
	}
}

void SceneBase::PostRender()
{
}

void SceneBase::RenderTextOnScreen(std::string text, float x, float y, Color color, float size)
{
	MGR_GRAPHICS.RenderTextOnScreen(TEXT, text, color, size, x, y);
}

void SceneBase::Exit()
{
	MGR_GRAPHICS.Drop();
}

void SceneBase::RenderLights()
{
	for (int i = 0; i < MGR_GRAPHICS.GetNumLightsDefined(); ++i)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(MGR_GRAPHICS.GetLightPos(i).x, MGR_GRAPHICS.GetLightPos(i).y, MGR_GRAPHICS.GetLightPos(i).z);
		MGR_GRAPHICS.RenderMesh(LIGHTBALL, MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();
	}
}
