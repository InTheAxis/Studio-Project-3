#include "MgrGraphics.h"
#include "Application.h"

void MgrGraphics::Init()
{
	//init opengl
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//init shaders, hardcoded lol
	m_shaders[LIT] = HelperShader::LoadShaders("Shader//default.vert", "Shader//lit.frag");
	m_shaders[UNLIT] = HelperShader::LoadShaders("Shader//default.vert", "Shader//unlit.frag");
	m_shaders[UNLIT_TINT] = HelperShader::LoadShaders("Shader//default.vert", "Shader//unlit_tint.frag");
	m_shaders[SIMPLE] = HelperShader::LoadShaders("Shader//default.vert", "Shader//simple.frag");
	m_shaders[LIT_BLEND] = HelperShader::LoadShaders("Shader//default.vert", "Shader//lit_blend.frag");
	m_shaders[WAVE_UNLIT_SCROLL] = HelperShader::LoadShaders("Shader//wave.vert", "Shader//unlit_scroll.frag");
	m_shaders[BILLBOARD_UNLIT] = HelperShader::LoadShaders("Shader//billboard.vert", "Shader//unlit.frag");
	UseShader(LIT);

	//init projection	
	m_customProj.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -100, 100);
	m_defaultProj.SetToPerspective(45.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
	m_projStack.LoadMatrix(m_defaultProj);

	//init default lights
	m_numLights = 1;
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		m_lights[i].enabled = false;
		m_lights[i].type = Light::LIGHT_POINT;
		m_lights[i].position.Set(0, 10, 0);
		m_lights[i].color.Set(1, 1, 1);
		m_lights[i].power = 1.f;
	}
	//m_lights[0].enabled = true;
	m_lights[0].type = Light::LIGHT_DIRECTIONAL;
}

void MgrGraphics::Drop()
{
	for (int i = 0; i < NUM_SHADERS; ++i)
	{
		glDeleteProgram(m_shaders[i]);
	}
}

void MgrGraphics::UseShader(SHADER program)
{
	glUseProgram(m_shaders[program]);
	m_currentShader = program;
}

void MgrGraphics::AttachCamera(CameraBase* cam)
{
	this->m_cameraPtr = cam;
}

void MgrGraphics::AttachModelStack(MS * modelStack)
{
	this->m_modelStackPtr = modelStack;
}

void MgrGraphics::PreRenderSetUp()
{
	glStencilMask(0xFF); //enable writing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Camera matrix
	m_viewStack.LoadIdentity();
	m_viewStack.LookAt(
		m_cameraPtr->position.x, m_cameraPtr->position.y, m_cameraPtr->position.z,
		m_cameraPtr->target.x, m_cameraPtr->target.y, m_cameraPtr->target.z,
		m_cameraPtr->up.x, m_cameraPtr->up.y, m_cameraPtr->up.z
	);
	m_modelStackPtr->LoadIdentity();
}

void MgrGraphics::SetUniformsMVP()
{
	Uniform_("model", m_modelStackPtr->Top());
	Uniform_("view", m_viewStack.Top());
	Uniform_("proj", m_projStack.Top());
}

void MgrGraphics::SetUniformsLight()
{
	Uniform_("numLights", m_numLights);
	if (m_numLights <= 0)
		return;
	char buf[64];
	for (int i = 0; i < m_numLights; ++i)
	{
		sprintf_s(buf, sizeof(buf), "lights[%i].enabled", i);
		Uniform_(buf, m_lights[i].enabled);
		if (!m_lights[i].enabled) continue;

		sprintf_s(buf, sizeof(buf), "lights[%i].type", i);
		Uniform_(buf, (int)m_lights[i].type);
		sprintf_s(buf, sizeof(buf), "lights[%i].color", i);
		Uniform_(buf, m_lights[i].color);
		sprintf_s(buf, sizeof(buf), "lights[%i].power", i);
		Uniform_(buf, m_lights[i].power);
		sprintf_s(buf, sizeof(buf), "lights[%i].kC", i);
		Uniform_(buf, m_lights[i].kC);
		sprintf_s(buf, sizeof(buf), "lights[%i].kL", i);
		Uniform_(buf, m_lights[i].kL);
		sprintf_s(buf, sizeof(buf), "lights[%i].kQ", i);
		Uniform_(buf, m_lights[i].kQ);
		sprintf_s(buf, sizeof(buf), "lights[%i].cosCutoff", i);
		Uniform_(buf, m_lights[i].cosCutoff);
		sprintf_s(buf, sizeof(buf), "lights[%i].cosInner", i);
		Uniform_(buf, m_lights[i].cosInner);
		sprintf_s(buf, sizeof(buf), "lights[%i].exponent", i);
		Uniform_(buf, m_lights[i].exponent);

		if (m_lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector4 lightDir(m_lights[i].position.x, m_lights[i].position.y, m_lights[i].position.z, 0);
			Vector3 lightDirection_cameraspace = m_viewStack.Top() * lightDir;
			sprintf_s(buf, sizeof(buf), "lights[%i].position_cameraspace", i);
			Uniform_(buf, lightDirection_cameraspace);
		}
		else if (m_lights[i].type == Light::LIGHT_SPOT)
		{
			Vector3 lightPosition_cameraspace = m_viewStack.Top() * Vector4(m_lights[i].position, 1);
			sprintf_s(buf, sizeof(buf), "lights[%i].position_cameraspace", i);
			Uniform_(buf, lightPosition_cameraspace);
			Vector3 spotDirection_cameraspace = m_viewStack.Top() * m_lights[i].spotDirection;
			sprintf_s(buf, sizeof(buf), "lights[%i].spotDirection", i);
			Uniform_(buf, spotDirection_cameraspace);
		}
		else //if (m_lights[i].type == Light::LIGHT_POINT)
		{
			Vector3 lightPosition_cameraspace = m_viewStack.Top() * Vector4(m_lights[i].position, 1);
			sprintf_s(buf, sizeof(buf), "lights[%i].position_cameraspace", i);
			Uniform_(buf, lightPosition_cameraspace);
		}
	}
}

void MgrGraphics::SetUniformsFog()
{	
	Uniform_("fog.enabled", m_fog.enabled);
	if (!m_fog.enabled)
		return;
	Uniform_("fog.type", m_fog.type);
	Uniform_("fog.color", m_fog.color);
	Uniform_("fog.start", m_fog.start);
	Uniform_("fog.end", m_fog.end);
	Uniform_("fog.density", m_fog.density);
}

void MgrGraphics::SetUniformsMesh(const CompMesh* mesh)
{
	if (m_currentShader == SIMPLE)
	{
		Uniform_("outColor", mesh->color);
		return;
	}

	//load material
	Uniform_("material.kAmbient", mesh->material.kAmbient);
	Uniform_("material.kDiffuse", mesh->material.kDiffuse);
	Uniform_("material.kSpecular", mesh->material.kSpecular);
	Uniform_("material.kShininess", mesh->material.kShininess);

	char buf[64];
	for (int i = 0; i < CompMesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			sprintf_s(buf, sizeof(buf), "colorTextureEnabled[%i]", i);
			Uniform_(buf, true);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			sprintf_s(buf, sizeof(buf), "colorTexture[%i]", i);
			Uniform_(buf, i);
		}
		else
		{
			sprintf_s(buf, sizeof(buf), "colorTextureEnabled[%i]", i);
			Uniform_(buf, false);
		}
	}
}

void MgrGraphics::SetUniformOffest(float vertOffset, float texOffset)
{		
	glUseProgram(m_shaders[WAVE_UNLIT_SCROLL]);
	HelperShader::SetUniform(m_shaders[WAVE_UNLIT_SCROLL], "vertOffset", vertOffset);
	HelperShader::SetUniform(m_shaders[WAVE_UNLIT_SCROLL], "texOffset", texOffset);
}

void MgrGraphics::SetUniformBlend(double dt)
{
	m_blend.elapsedTime += dt;

	if (m_blend.elapsedTime > m_blend.dura)
		m_blend.enabled = false;

	glUseProgram(m_shaders[LIT_BLEND]);
	HelperShader::SetUniform(m_shaders[LIT_BLEND], "blend.enabled", m_blend.enabled);
	HelperShader::SetUniform(m_shaders[LIT_BLEND], "blend.pos", m_blend.pos);	
	HelperShader::SetUniform(m_shaders[LIT_BLEND], "blend.radius", m_blend.radius);	
	HelperShader::SetUniform(m_shaders[LIT_BLEND], "blend.cutoff", m_blend.cutoff);
}

void MgrGraphics::RenderMesh(CompMesh * mesh, SHADER shader, bool outline)
{
	UseShader(shader);

	SetUniformsMVP();
	SetUniformsLight();
	SetUniformsFog();
	SetUniformsMesh(mesh);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	if (outline)
		glStencilMask(0xFF);
	else
		glStencilMask(0x00);
	
	DrawMesh(mesh);

	for (int i = 0; i < CompMesh::MAX_TEXTURES; ++i)
		if (mesh->textureArray[i] > 0)
			glBindTexture(GL_TEXTURE_2D, 0);

	if (outline)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //don't draw if equal 1
		glStencilMask(0x00); //disable writes		
		UseShader(SIMPLE);
		m_modelStackPtr->PushMatrix();
		m_modelStackPtr->Scale(1.1f, 1.1f, 1.1f);
		SetUniformsMVP();
		m_modelStackPtr->PopMatrix();
		SetUniformsMesh(mesh);
		DrawMesh(mesh);
		glStencilMask(0xFF); //enable writes	
		glClear(GL_STENCIL_BUFFER_BIT);
	}
}

void MgrGraphics::RenderSpriteAnim(CompMesh * mesh, SHADER shader, bool outline)
{
	UseShader(shader);

	SetUniformsMVP();
	SetUniformsLight();
	SetUniformsFog();
	SetUniformsMesh(mesh);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	if (outline)
		glStencilMask(0xFF);
	else
		glStencilMask(0x00);

	DrawSpriteAnim(static_cast<CompSpriteAnim*>(mesh));

	for (int i = 0; i < CompMesh::MAX_TEXTURES; ++i)
		if (mesh->textureArray[i] > 0)
			glBindTexture(GL_TEXTURE_2D, 0);

	if (outline)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //don't draw if equal 1
		glStencilMask(0x00); //disable writes		
		UseShader(SIMPLE);
		m_modelStackPtr->PushMatrix();
		m_modelStackPtr->Scale(1.1f, 1.1f, 1.1f);
		SetUniformsMVP();
		m_modelStackPtr->PopMatrix();
		SetUniformsMesh(mesh);
		DrawMesh(mesh);
		glStencilMask(0xFF); //enable writes	
		glClear(GL_STENCIL_BUFFER_BIT);
	}
}

void MgrGraphics::RenderMeshOnScreen(CompMesh * mesh, SHADER shader, float size, float x, float y)
{
	Mtx44 ortho;
	int w = Application::GetWindowWidth() * 0.5f;
	int h = Application::GetWindowHeight() * 0.5f;
	ortho.SetToOrtho(-w, w, -h, h, -10000, 10000);
	m_projStack.PushMatrix();
	m_projStack.LoadMatrix(ortho);
	m_viewStack.PushMatrix();
	m_viewStack.LoadIdentity();
	m_modelStackPtr->PushMatrix();
	//m_modelStackPtr->LoadIdentity();
	m_modelStackPtr->Translate(x, y, 0);
	m_modelStackPtr->Scale(size, size, size);

	RenderMesh(mesh, shader, false);

	m_modelStackPtr->PopMatrix();
	m_viewStack.PopMatrix();
	m_projStack.PopMatrix();
}

void MgrGraphics::RenderText(CompMesh * mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	UseShader(UNLIT_TINT);

	glDisable(GL_DEPTH_TEST);	
	Uniform_("tintColor", color);
	SetUniformsMesh(mesh);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		m_modelStackPtr->PushMatrix();
		m_modelStackPtr->Translate(i * 0.65f + 0.5f, 0.5f, 0);
		SetUniformsMVP();
		m_modelStackPtr->PopMatrix();

		DrawMesh(mesh, 6, (unsigned)text[i] * 6);
	}	
	glEnable(GL_DEPTH_TEST);
}

void MgrGraphics::RenderTextOnScreen(CompMesh * mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	Mtx44 ortho;
	int w = Application::GetWindowWidth() * 0.1f;
	int h = Application::GetWindowHeight() * 0.1f;
	ortho.SetToOrtho(0, w, 0, h, -10, 10);
	m_projStack.PushMatrix();
	m_projStack.LoadMatrix(ortho);
	m_viewStack.PushMatrix();
	m_viewStack.LoadIdentity();
	m_modelStackPtr->PushMatrix();
	m_modelStackPtr->LoadIdentity();
	m_modelStackPtr->Translate(x, y, 0);
	m_modelStackPtr->Scale(size, size, size);

	RenderText(mesh, text, color);

	m_modelStackPtr->PopMatrix();
	m_viewStack.PopMatrix();
	m_projStack.PopMatrix();
}

void MgrGraphics::DefineCustomProjection(Mtx44& proj)
{
	m_customProj = proj;
	m_projStack.LoadMatrix(m_customProj);
}

void MgrGraphics::DefineNumLights(int zeroToEight)
{
	m_numLights = Math::Clamp(zeroToEight, 0, MAX_LIGHTS);
}

void MgrGraphics::DefineLightProps(int index, bool enabled, Light::LIGHT_TYPE type, Position pos, Color color, float power)
{
	int i = Math::Clamp(index, 0, MAX_LIGHTS);
	m_lights[i].enabled = enabled;
	if (type != Light::LIGHT_NULL)
		m_lights[i].type = type;
	if (pos != NULL)
		m_lights[i].position = pos;
	if (color != NULL)
		m_lights[i].color = color;
	if (power != NULL)
		m_lights[i].power = power;
}

void MgrGraphics::DefineFogProps(bool enabled, Fog::FOG_TYPE type, Vector4 color, float density, float start, float end)
{
	m_fog.enabled = enabled;
	if (type != Fog::FOG_NULL)
		m_fog.type = type;
	if (color != NULL)
		m_fog.color = color;
	if (density != NULL)
		m_fog.density = density;
	if (start != NULL)
		m_fog.start = start;
	if (end != NULL)
		m_fog.end = end;}

void MgrGraphics::DefineBlendProps(bool enabled, Vector3 pos, float radius, float dura, float cutoff)
{
	m_blend.elapsedTime = 0.0;
	
	m_blend.enabled = enabled;
	m_blend.pos = pos;
	m_blend.radius = radius;
	m_blend.dura = dura;
	if (cutoff > 0)
		m_blend.cutoff = cutoff;
}

int MgrGraphics::GetNumLightsDefined()
{	
	return m_numLights;
}

Vector3 MgrGraphics::GetLightPos(int index)
{
	int i = Math::Clamp(index, 0, MAX_LIGHTS);
	return m_lights[i].position;
}

void MgrGraphics::DrawMesh(CompMesh* mesh)
{
	DrawMesh(mesh, mesh->indexSize, 0);
}

void MgrGraphics::DrawMesh(CompMesh* mesh, unsigned count, unsigned offset)
{
	glBindVertexArray(mesh->vao);
	if (mesh->mode == CompMesh::DRAW_LINES)
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else if (mesh->mode == CompMesh::DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}

void MgrGraphics::DrawSpriteAnim(CompSpriteAnim * anim)
{
	glBindVertexArray(anim->vao);
	if (anim->mode == CompMesh::DRAW_LINES)
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(anim->m_currentFrame * 6 * sizeof(GLuint)));
	else if (anim->mode == CompMesh::DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(anim->m_currentFrame * 6 * sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(anim->m_currentFrame * 6 * sizeof(GLuint)));
}
