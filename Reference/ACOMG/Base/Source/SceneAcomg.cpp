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
	m_meshList[MESH_BILLBOARD]->textureArray[0] = LoadTGA("Image//Acomg//scoreboard.tga");

	m_meshList[MESH_PARTICLE_SMOKE] = MeshBuilder::GenerateSpriteAnimation("paticle smoke", 1, 6);
	m_meshList[MESH_PARTICLE_SMOKE]->textureArray[0] = LoadTGA("Image//Acomg//particleAnim.tga");

	m_meshList[MESH_PARTICLE_RED] = MeshBuilder::GenerateSpriteAnimation("paticle smoke", 1, 1);
	m_meshList[MESH_PARTICLE_RED]->textureArray[0] = LoadTGA("Image//Acomg//particleRed.tga");

	m_meshList[MESH_PARTICLE_BLUE] = MeshBuilder::GenerateSpriteAnimation("paticle smoke", 1, 1);
	m_meshList[MESH_PARTICLE_BLUE]->textureArray[0] = LoadTGA("Image//Acomg//particleBlue.tga");
	
	m_meshList[MESH_LIGHT_DEPTH] = MeshBuilder::GenerateQuad("light depth", Color(1, 1, 1), 2);
	m_meshList[MESH_LIGHT_DEPTH]->textureArray[0] = lightDepthFBO.GetTexture();

	m_meshList[MESH_FLOAT_QUAD] = MeshBuilder::GenerateQuad("float", Color(1, 1, 1), 2);
	m_meshList[MESH_FLOAT_QUAD]->textureArray[0] = hdrFBO.GetTexture();

	m_meshList[MESH_BRIGHT_QUAD] = MeshBuilder::GenerateQuad("bright", Color(1, 1, 1), 2);
	m_meshList[MESH_BRIGHT_QUAD]->textureArray[0] = brightFBO.GetTexture();
	
	m_meshList[MESH_BLUR_QUAD] = MeshBuilder::GenerateQuad("blur", Color(1, 1, 1), 2);
	m_meshList[MESH_BLUR_QUAD]->textureArray[0] = hdrFBO.GetTexture();

	m_meshList[MESH_FINAL_QUAD] = MeshBuilder::GenerateQuad("blur", Color(1, 1, 1), 2);
	m_meshList[MESH_FINAL_QUAD]->textureArray[0] = hdrFBO.GetTexture();

	CompSpriteAnim *sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_TITLE]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 1, 1.f, true); //Startframe, endframe, repeat, time, active		
	}

	sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_PARTICLE_SMOKE]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 1, 1.f, true); //Startframe, endframe, repeat, time, active		
		smoke.Init(sa, 1);
	}

	sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_PARTICLE_RED]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 0, 1, 1.f, false); //Startframe, endframe, repeat, time, active		
		fireR.Init(sa, 2);
	}

	sa = static_cast<CompSpriteAnim*>(m_meshList[MESH_PARTICLE_BLUE]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 0, 1, 1.f, false); //Startframe, endframe, repeat, time, active		
		fireB.Init(sa, 3);
	}
}

void SceneAcomg::Init()
{
	SceneBase::Init();

	lightDepthFBO.Init(2048, 2048);
	hdrFBO.Init(Application::GetWindowWidth(), Application::GetWindowHeight());
	brightFBO.Init(Application::GetWindowWidth(), Application::GetWindowHeight());
	blurFBO[0].Init(Application::GetWindowWidth(), Application::GetWindowHeight());
	blurFBO[1].Init(Application::GetWindowWidth(), Application::GetWindowHeight());

	InitMeshes();

	//define stuff in MgrGraphics
	MGR_GRAPHICS.DefineNumLights(1);
	MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_DIRECTIONAL, Position(4, 6, -4), Color(0.6f, 0.6f, 1.4f, 1));
	//MGR_GRAPHICS.DefineLightProps(1, true, Light::LIGHT_POINT, Position(0, 230, -10), Color(1, 1, 1, 1), 2.f);	
	MGR_GRAPHICS.DefineFogProps(true, Fog::FOG_EXPO_SQ, Color(0.1f, 0.f, 0.f), 0.0005f);	

	lightMoveX = 20;
}

void SceneAcomg::Update(double dt)
{	
	SceneBase::PreUpdate(dt);

	//Debugging options
	if (1)
	{	
		enableBloom = !(Application::IsKeyPressed('B'));
		showFBO = Application::IsKeyPressed('N');

		//if (Application::IsKeyPressed(VK_F5))
		//	MGR_GRAPHICS.DefineNumLights(0);
		//if (Application::IsKeyPressed(VK_F6))
		//	MGR_GRAPHICS.DefineNumLights(1);
		//if (Application::IsKeyPressed(VK_F7))
		//	MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_POINT);
		//else if (Application::IsKeyPressed(VK_F8))
		//	MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_DIRECTIONAL);
		//else if (Application::IsKeyPressed(VK_F9))
		//	MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_SPOT);

		//const static float LSPEED = 100.f;
		Vector3 newLightPos = MGR_GRAPHICS.GetLightPos(0);
		//if (Application::IsKeyPressed('I'))
		//	newLightPos.z -= (float)(LSPEED * dt);
		//if (Application::IsKeyPressed('K'))
		//	newLightPos.z += (float)(LSPEED * dt);
		//if (Application::IsKeyPressed('J'))
		//	newLightPos.x -= (float)(LSPEED * dt);
		//if (Application::IsKeyPressed('L'))
		//	newLightPos.x += (float)(LSPEED * dt);
		//if (Application::IsKeyPressed('O'))
		//	newLightPos.y -= (float)(LSPEED * dt);
		//if (Application::IsKeyPressed('U'))
		//	newLightPos.y += (float)(LSPEED * dt);
		
		if (newLightPos.x < -8 && lightMoveX < 0)
		{
			lightMoveX = -lightMoveX;			
		}
		else if (newLightPos.x > 8 && lightMoveX > 0)
		{
			lightMoveX = -lightMoveX;			
		}
		
		newLightPos.x += lightMoveX * dt;

		MGR_GRAPHICS.DefineLightProps(0, true, Light::LIGHT_NULL, newLightPos);
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

	smoke.Update(dt);
	fireR.Update(dt);
	fireB.Update(dt);

	SceneBase::PostUpdate(dt);
}

void SceneAcomg::Render()
{
	lightDepthFBO.BindForWriting();
	ShadowPass();
	
	hdrFBO.BindForWriting();	
	HdrPass();
	
	brightFBO.BindForWriting();	
	lightDepthFBO.BindForReading(GL_TEXTURE8);
	hdrFBO.BindForReading(GL_TEXTURE0);
	BrightPass();
		
	BlurPass();

	BindDeafultFBO();	
	hdrFBO.BindForReading(GL_TEXTURE0);
	blurFBO[0].BindForReading(GL_TEXTURE1);
	FinalPass();
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

void SceneAcomg::RenderPlatform(MgrGraphics::SHADER shader)
{
	m_modelStack.PushMatrix();	
	m_modelStack.Scale(100, 1, 100);
	m_modelStack.Rotate(-90, 1, 0, 0);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_PLATFORM], shader, true);
	m_modelStack.PopMatrix();
}

void SceneAcomg::RenderSkyPlane()
{
	m_modelStack.PushMatrix();
	m_modelStack.Translate(0, 4500, 0);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_SKYPLANE], MGR_GRAPHICS.UNLIT);
	m_modelStack.PopMatrix();
}

void SceneAcomg::RenderTerrain(MgrGraphics::SHADER shader)
{
	m_modelStack.PushMatrix();	
	m_modelStack.Translate(0, -0.1f, 0);
	m_modelStack.Scale(5000, 800, 5000);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_TERRAIN], shader);
	m_modelStack.PopMatrix();
}

void SceneAcomg::ShadowPass()
{
	SceneBase::PreRender();
	RenderWorld(P_DEPTH);
}

void SceneAcomg::HdrPass()
{
	SceneBase::PreRender();
	RenderWorld(P_HDR);	
	SceneBase::PostRender();
}

void SceneAcomg::BrightPass()
{
	SceneBase::PreRender();
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_FLOAT_QUAD], MGR_GRAPHICS.BRIGHT);		
}

void SceneAcomg::BlurPass()
{
	bool horizontal = false;
	int amount = 10;	
	for (unsigned int i = 0; i < amount; i++)
	{		
		blurFBO[horizontal].BindForWriting();
		MGR_GRAPHICS.SetUniformBlur(horizontal);
		if (i == 0)
		{
			brightFBO.BindForReading(GL_TEXTURE0);
			m_meshList[MESH_BLUR_QUAD]->textureArray[0] = brightFBO.GetTexture();
		}
		else
		{
			blurFBO[!horizontal].BindForReading(GL_TEXTURE0);
			m_meshList[MESH_BLUR_QUAD]->textureArray[0] = blurFBO[!horizontal].GetTexture();
		}
		SceneBase::PreRender();
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BLUR_QUAD], MGR_GRAPHICS.BLUR);	
		horizontal = !horizontal;
	}

	m_meshList[MESH_FINAL_QUAD]->textureArray[0] = hdrFBO.GetTexture();
	m_meshList[MESH_FINAL_QUAD]->textureArray[1] = blurFBO[1].GetTexture();
}


void SceneAcomg::FinalPass()
{
	SceneBase::PreRender();
	RenderWorld(P_FINAL);
}

void SceneAcomg::RenderWorld(RENDER_PASS renderPass)
{	
	MgrGraphics::SHADER lit = MGR_GRAPHICS.LIT;
	MgrGraphics::SHADER litBlend = MGR_GRAPHICS.LIT_BLEND;	
	MgrGraphics::SHADER unlit = MGR_GRAPHICS.UNLIT;	
	if (renderPass == P_DEPTH)
	{
		lit = litBlend = unlit = MGR_GRAPHICS.DEPTH;
	}
	else if (renderPass == P_FINAL)
	{		
		if (showFBO)
		{
			m_modelStack.PushMatrix();
			m_modelStack.Translate(-0.5f, 0.5f, 0);
			m_modelStack.Scale(0.5f, 0.5f, 1);
			MGR_GRAPHICS.RenderMesh(m_meshList[MESH_LIGHT_DEPTH], MGR_GRAPHICS.FBO);
			m_modelStack.PopMatrix();

			m_modelStack.PushMatrix();
			m_modelStack.Translate(0.5f, 0.5f, 0);
			m_modelStack.Scale(0.5f, 0.5f, 1);
			MGR_GRAPHICS.RenderMesh(m_meshList[MESH_FLOAT_QUAD], MGR_GRAPHICS.FBO);
			m_modelStack.PopMatrix();

			m_modelStack.PushMatrix();
			m_modelStack.Translate(-0.5f, -0.5f, 0);
			m_modelStack.Scale(0.5f, 0.5f, 1);
			MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BRIGHT_QUAD], MGR_GRAPHICS.FBO);
			m_modelStack.PopMatrix();

			m_modelStack.PushMatrix();
			m_modelStack.Translate(0.5f, -0.5f, 0);
			m_modelStack.Scale(0.5f, 0.5f, 1);
			MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BLUR_QUAD], MGR_GRAPHICS.FBO);
			m_modelStack.PopMatrix();
		}
		else
			MGR_GRAPHICS.RenderMesh(m_meshList[MESH_FINAL_QUAD], enableBloom ? MGR_GRAPHICS.BLOOM : MGR_GRAPHICS.FBO);

		//On screen text
		if (DEBUG_)
		{
			////Render the crosshair
			glLineWidth(2);
			m_modelStack.Translate(0, 0, 10);
			MGR_GRAPHICS.RenderMeshOnScreen(m_meshList[MESH_CROSSHAIR], MGR_GRAPHICS.UNLIT, 10.f);
			glLineWidth(1);

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
		return;
	}

	RenderSkyPlane();
	RenderTerrain(lit);

	m_modelStack.PushMatrix();
	m_modelStack.Translate(0, 11.f, 0);
	m_modelStack.Scale(10, 2.5f, 10);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_WATER], MGR_GRAPHICS.WAVE_UNLIT_SCROLL);
	m_modelStack.PopMatrix();	

	m_modelStack.PushMatrix();
	{
		m_modelStack.Translate(0, 200, 0);

		RenderPlatform(MGR_GRAPHICS.LIT_BLEND);
		m_modelStack.PushMatrix();
		m_modelStack.Translate(-40, 25, -350);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_CUBE_B], lit, true);
		m_modelStack.PopMatrix();
		m_modelStack.PushMatrix();
		m_modelStack.Translate(40, 25, -350);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_CUBE_R], lit, true);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, -100, -450);
		m_modelStack.Rotate(180, 0, 1, 0);
		m_modelStack.Scale(10, 5, 20);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_STAGE], lit);
		m_modelStack.PopMatrix();


		m_modelStack.PushMatrix();
		m_modelStack.Translate(0, 1000, -1000);
		m_modelStack.Scale(500, 500, 500);
		MGR_GRAPHICS.RenderSpriteAnim(m_meshList[MESH_TITLE], unlit);
		m_modelStack.PopMatrix();

		m_modelStack.PushMatrix();
		m_modelStack.Translate(-40, 15, -320);
		m_modelStack.Rotate(180, 0, 1, 0);
		m_modelStack.Rotate(-20, 1, 0, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BLADE_B], lit);
		m_modelStack.PopMatrix();
		m_modelStack.PushMatrix();
		m_modelStack.Translate(40, 15, -320);
		m_modelStack.Rotate(180, 0, 1, 0);
		m_modelStack.Rotate(-20, 1, 0, 0);
		MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BLADE_R], lit);
		m_modelStack.PopMatrix();

	}
	m_modelStack.PopMatrix();

	m_modelStack.PushMatrix();
	m_modelStack.Translate(100, 250, -150);
	m_modelStack.Scale(50, 50, 1);
	MGR_GRAPHICS.RenderMesh(m_meshList[MESH_BILLBOARD], MGR_GRAPHICS.BILLBOARD_UNLIT);
	m_modelStack.PopMatrix();

	//render particles
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	fireR.Render(&m_modelStack, &m_camera, renderPass == P_DEPTH);
	fireB.Render(&m_modelStack, &m_camera, renderPass == P_DEPTH);
	smoke.Render(&m_modelStack, &m_camera, renderPass == P_DEPTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneAcomg::BindDeafultFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
