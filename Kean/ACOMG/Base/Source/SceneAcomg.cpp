#include "SceneAcomg.h"
#include "Application.h"

SceneAcomg::SceneAcomg()
{
}

SceneAcomg::~SceneAcomg()
{
}

void SceneAcomg::InitMeshes()
{	
	m_meshList[MESH_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair", 0, 1, 0);
			   
	m_meshList[MESH_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair", 0, 1, 0);
	m_meshList[MESH_SKYPLANE] = MeshBuilder::GenerateSkyPlane("skyplane", Color(1, 1, 1), 256, 500.f, 5000.f, 10, 10);
	m_meshList[MESH_SKYPLANE]->textureArray[0] = LoadTGA("Image//Acomg//skyplane.tga");
	m_meshList[MESH_TERRAIN] = MeshBuilder::GenerateTerrain("terrain", "Image//Acomg//heightmap.raw", m_heightMap, 5000, 2200, 5000);
	m_meshList[MESH_TERRAIN]->textureArray[0] = LoadTGA("Image//Acomg//terrainFlat.tga");
	m_meshList[MESH_TERRAIN]->textureArray[1] = LoadTGA("Image//Acomg//terrainRough.tga");
	m_meshList[MESH_TERRAIN]->textureArray[2] = LoadTGA("Image//Acomg//terrainMoss.tga");

	m_meshList[MESH_PLATFORM] = MeshBuilder::GenerateQuad("platform", Color(0.9f, 0.9f, 1));
	m_meshList[MESH_PLATFORM]->textureArray[0] = LoadTGA("Image//Acomg//platform.tga");
	m_meshList[MESH_PLATFORM]->textureArray[1] = LoadTGA("Image//Acomg//platformGrey.tga");
	m_meshList[MESH_PLATFORM]->material.kShininess = 32.f;
	m_meshList[MESH_CUBE_R] = MeshBuilder::GenerateOBJ("cube red", "OBJ//Acomg//Block.obj", Color(1, 0, 0.3f));
	m_meshList[MESH_CUBE_R]->textureArray[0] = LoadTGA("Image//Acomg//blockRed.tga");
	m_meshList[MESH_CUBE_B] = MeshBuilder::GenerateOBJ("cube blue", "OBJ//Acomg//Block.obj", Color(0, 0.3f, 1));
	m_meshList[MESH_CUBE_B]->textureArray[0] = LoadTGA("Image//Acomg//blockBlue.tga");
			   
	m_meshList[MESH_TITLE] = MeshBuilder::GenerateSpriteAnimation("title", 1, 6);
	m_meshList[MESH_TITLE]->textureArray[0] = LoadTGA("Image//Acomg//title.tga");

	m_meshList[MESH_STAGE] = MeshBuilder::GenerateOBJ("stage", "OBJ//Acomg//Stage.obj", Color(0.9f, 0.9f, 1));
	m_meshList[MESH_STAGE]->textureArray[0] = LoadTGA("Image//Acomg//platform.tga");	

	m_meshList[MESH_BLADE_R] = MeshBuilder::GenerateOBJ("blade red", "OBJ//Acomg//Blade.obj", Color(1, 0, 0.3f));
	m_meshList[MESH_BLADE_R]->textureArray[0] = LoadTGA("Image//Acomg//bladeRed.tga");
	m_meshList[MESH_BLADE_B] = MeshBuilder::GenerateOBJ("blade blue", "OBJ//Acomg//Blade.obj", Color(0, 0.3f, 1));
	m_meshList[MESH_BLADE_B]->textureArray[0] = LoadTGA("Image//Acomg//bladeBlue.tga");

	m_meshList[MESH_WATER] = MeshBuilder::GeneratePlane("sea", Color(1, 1, 1), 200, 200, 10);
	m_meshList[MESH_WATER]->textureArray[0] = LoadTGA("Image//Acomg//ocean.tga");	

	m_meshList[MESH_BILLBOARD] = MeshBuilder::GenerateQuad("billboard", Color(1, 1, 1), 1);
	m_meshList[MESH_BILLBOARD]->textureArray[0] = LoadTGA("Image//tree.tga");

	m_meshList[MESH_PARTICLE_WATER] = MeshBuilder::GenerateSpriteAnimation("paticle water", 1, 6);
	m_meshList[MESH_PARTICLE_WATER]->textureArray[0] = LoadTGA("Image//Acomg//particleAnim.tga");
	

	CompSpriteAnim *sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_TITLE]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 1, 1.f, true); //Startframe, endframe, repeat, time, active		
	}

	sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_PARTICLE_WATER]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 1, 1.f, true); //Startframe, endframe, repeat, time, active		
		particleList.Init(sa);
	}
}

void SceneAcomg::Init()
{
	SceneBase::Init();

	InitMeshes();

	//define stuff in MgrGraphics
	MGR_GRAPHICS.DefineNumLights(4);
	MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_DIRECTIONAL, Position(0, 10, 2), Color(1, 1, 1, 1), 1.f);
	MGR_GRAPHICS.DefineLightProps(1, true, Light::LIGHT_POINT, Position(0, 230, -10), Color(1, 1, 1, 1), 2.f);	
	MGR_GRAPHICS.DefineFogProps(true, Fog::FOG_EXPO_SQ, Color(0.1f, 0.f, 0.f), 0.0005f);	
}

void SceneAcomg::Update(double dt)
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
		Vector3 newLightPos = MGR_GRAPHICS.GetLightPos(1);
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

		MGR_GRAPHICS.DefineLightProps(1, true, Light::LIGHT_NULL, newLightPos);
	}

	CompSpriteAnim *sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_TITLE]);
	if (sa)
	{
		sa->Update(dt);		
		sa->m_anim->animActive = true;
	}

	if (Application::IsKeyPressed(VK_LBUTTON))
		MGR_GRAPHICS.DefineBlendProps(true, Vector3(m_camera.position.x, 200, m_camera.position.z), 5, 2, 0);

	MGR_GRAPHICS.SetUniformOffest(m_elapsedTime, m_elapsedTime * 0.25f);
	MGR_GRAPHICS.SetUniformBlend(dt);

	particleList.Update(dt);

	SceneBase::PostUpdate(dt);
}

void SceneAcomg::Render()
{
	SceneBase::PreRender();

	RenderSkyPlane();
	RenderTerrain();
	
	m_modelStack.PushMatrix();
	m_modelStack.Translate(0, 11.f, 0);
	m_modelStack.Scale(10, 2.5f, 10);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_WATER], MGR_GRAPHICS.WAVE_UNLIT_SCROLL);
	m_modelStack.PopMatrix();

	m_modelStack.PushMatrix();
	{
		m_modelStack.Translate(0, 200, 0);

		RenderPlatform();
		m_modelStack.PushMatrix();
		m_modelStack.Translate(-40, 20, -200);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_CUBE_B], MGR_GRAPHICS.LIT, true);		
		m_modelStack.PopMatrix();
		m_modelStack.PushMatrix();
		m_modelStack.Translate(40, 20, -200);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_CUBE_R], MGR_GRAPHICS.LIT, true);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, -100, -450);
		m_modelStack.Rotate(180, 0, 1, 0);
		m_modelStack.Scale(10, 5, 20);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_STAGE], MGR_GRAPHICS.LIT);
		m_modelStack.PopMatrix();


		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 1000, -1000);
		m_modelStack.Scale(500, 500, 500);
		MGR_GRAPHICS.RenderSpriteAnim(m_meshList[MESH_TITLE], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, -100, 0);
		m_modelStack.Scale(100, 100, 1);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BILLBOARD], MGR_GRAPHICS.BILLBOARD_UNLIT);
		m_modelStack.PopMatrix();

	}
	m_modelStack.PopMatrix();

	m_modelStack.PushMatrix();
	m_modelStack.Translate(-500, -50, 1);
	m_modelStack.Rotate(-90, 1, 0, 0);
	m_modelStack.Rotate(10, 0, 1, 0);
	MGR_GRAPHICS.RenderMeshOnScreen(m_meshList[MESH_BLADE_B], MGR_GRAPHICS.LIT, 12.f);
	m_modelStack.PopMatrix();
	m_modelStack.PushMatrix();
	m_modelStack.Translate(500, -50, 1);
	m_modelStack.Rotate(-90, 1, 0, 0);
	m_modelStack.Rotate(-10, 0, 1, 0);
	MGR_GRAPHICS.RenderMeshOnScreen(m_meshList[MESH_BLADE_R], MGR_GRAPHICS.LIT, 12.f);
	m_modelStack.PopMatrix();

	//render particles
	particleList.Render(&m_modelStack, &m_camera);

	////Render the crosshair
	glLineWidth(2);
	MGR_GRAPHICS.RenderMeshOnScreen(m_meshList[MESH_CROSSHAIR], MGR_GRAPHICS.UNLIT, 10.f);
	glLineWidth(1);

	//On screen text
	if (DEBUG_)
	{
		std::ostringstream ss;

		RenderTextOnScreen("BEET SABRE", 0.5f, 0);

		ss.str("");
		ss.precision(5);
		ss << "FPS: " << m_fps;
		RenderTextOnScreen(ss.str(), 0.5f, 6);

		ss.str("");
		ss.precision(4);
		ss << "Camera" << m_camera.position;
		RenderTextOnScreen(ss.str(), 0.5f, 3);
	}
	SceneBase::PostRender();
}

void SceneAcomg::Exit()
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		if (m_meshList[i])
			delete m_meshList[i];
	}

	SceneBase::Exit();
}

void SceneAcomg::RenderPlatform()
{
	m_modelStack.PushMatrix();	
	m_modelStack.Scale(100, 1, 100);
	m_modelStack.Rotate(-90, 1, 0, 0);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_PLATFORM], MGR_GRAPHICS.LIT_BLEND, true);
	m_modelStack.PopMatrix();
}

void SceneAcomg::RenderSkyPlane()
{
	m_modelStack.PushMatrix();
	m_modelStack.Translate(0, 4500, 0);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_SKYPLANE], MGR_GRAPHICS.UNLIT);
	m_modelStack.PopMatrix();
}

void SceneAcomg::RenderTerrain()
{
	m_modelStack.PushMatrix();	
	m_modelStack.Translate(0, -0.1f, 0);
	m_modelStack.Scale(5000, 800, 5000);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_TERRAIN], MGR_GRAPHICS.LIT);
	m_modelStack.PopMatrix();
}