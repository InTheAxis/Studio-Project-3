#include "MgrGraphics.h"
#include "../../Utility/WinDebug.h"
#include "../../Utility/Input/ControllerKeyboard.h"
#include "../../Utility/Graphics/Mesh.h"
#include "../../Utility/Graphics/Material.h"
#include "../../Node/Components/Renderable.h"
#include "../../Application.h"
		  
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

MgrGraphics::MgrGraphics(std::string name) : Manager<MgrGraphics>(name)
{
}

MgrGraphics::~MgrGraphics()
{
	shaderPrograms.clear();
	cachedUniforms.clear();
	for (auto mesh : cachedMeshes)
	{
		if (mesh.second)
			delete mesh.second;
		mesh.second = nullptr;
	}
	cachedMeshes.clear();
	for (auto mat : cachedMaterials)
	{
		if (mat.second)
			delete mat.second;
		mat.second = nullptr;
	}
	cachedMaterials.clear();

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtDumpMemoryLeaks();
}

void MgrGraphics::Start()
{
	Debug::Log("Initializing OpenGL");
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

	modelStack.LoadIdentity();
	defaultView.SetToLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	view = &defaultView;
	projStack.LoadIdentity();
	SetProjPerspective();
	lightDir.Set(1, 1, 0, 1);

	Debug::Log("Loading shaders...");	
	shaderPrograms[UNLIT] = Resource::LoadShaders("shader/default.vert", "shader/unlit.frag");
	shaderPrograms[SIMPLE] = Resource::LoadShaders("shader/simple.vert", "shader/simple.frag");
	shaderPrograms[LIT] = Resource::LoadShaders("shader/default.vert", "shader/lit.frag");
	shaderPrograms[HSV_LIT] = Resource::LoadShaders("shader/default.vert", "shader/hsv_lit.frag");
	shaderPrograms[HSV_UNLIT] = Resource::LoadShaders("shader/default.vert", "shader/hsv_unlit.frag");
	shaderPrograms[COLOR_SPOT] = Resource::LoadShaders("shader/simple.vert", "shader/colorSpot.frag");

	Node::Start();
}
void MgrGraphics::Update(double dt)
{
	if (ControllerKeyboard::Instance()->IsKeyPressed('0'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (ControllerKeyboard::Instance()->IsKeyPressed('1'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Node::Update(dt);
}

void MgrGraphics::End()
{
	Node::End();
}

void MgrGraphics::PreRender()
{
	glStencilMask(0xFF); //enable writing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	UseShader(UNLIT);
	modelStack.LoadIdentity();
	glLineWidth(3);
}

void MgrGraphics::SetUniformScene()
{
	//constant uniforms across shaders/meshess
	
	SetUniform("view", *view);
	SetUniform("proj", projStack.Top());
	SetUniform("lightDir", lightDir);
	SetUniform("scrollAmt", Vector3());
}

void MgrGraphics::UseShader(MgrGraphics::SHADER shader)
{
	currShader = shader;
	glUseProgram(shaderPrograms[currShader]);
	SetUniformScene();
}

void MgrGraphics::SetUniform(std::string uniform, int i, MgrGraphics::SHADER shader)
{
	glUniform1i(GetUniLoc(uniform, shader), i);
}

void MgrGraphics::SetUniform(std::string uniform, bool b, MgrGraphics::SHADER shader)
{
	glUniform1i(GetUniLoc(uniform, shader), b);
}

void MgrGraphics::SetUniform(std::string uniform, const Mtx44& m, MgrGraphics::SHADER shader)
{
	glUniformMatrix4fv(GetUniLoc(uniform, shader), 1, GL_FALSE, &(m.a[0]));
}

void MgrGraphics::SetUniform(std::string uniform, const Vector3& v, MgrGraphics::SHADER shader)
{
	glUniform3fv(GetUniLoc(uniform, shader), 1, &v.x);
}

void MgrGraphics::SetUniform(std::string uniform, const Vector4& v, MgrGraphics::SHADER shader)
{
	glUniform4fv(GetUniLoc(uniform, shader), 1, &v.x);
}

void MgrGraphics::SetUniform(std::string uniform, float f, MgrGraphics::SHADER shader)
{
	glUniform1f(GetUniLoc(uniform, shader), f);
}

MgrGraphics::SHADER MgrGraphics::GetCurrShader()
{
	return currShader;
}

unsigned MgrGraphics::GetUniLoc(std::string uniform, MgrGraphics::SHADER shader)
{
	if (shader != CURRENT && currShader != shader)
		UseShader(shader);

	MgrGraphics::SHADER shad = (shader == CURRENT ? currShader : shader);

	//return glGetUniformLocation(shader == CURRENT ? shaderPrograms[currShader] : shaderPrograms[shader], uniform.c_str());
	if (cachedUniforms[shad].count(uniform) <= 0)
		cachedUniforms[shad][uniform] = glGetUniformLocation(shaderPrograms[shad], uniform.c_str());
	return cachedUniforms[shad][uniform];
}

void MgrGraphics::CacheMesh(Mesh * mesh)
{
	cachedMeshes[mesh->name] = mesh;
}

Mesh* MgrGraphics::GetCachedMesh(std::string name)
{
	return cachedMeshes[name];
}

void MgrGraphics::CacheMaterial(Material* mat)
{
	cachedMaterials[mat->name] = mat;
}
Material* MgrGraphics::GetCachedMaterial(std::string name)
{
	return cachedMaterials[name];
}

MS* MgrGraphics::GetModelStack()
{
	return &modelStack;
}

Mtx44 MgrGraphics::GetView()
{
	return *view;
}

MS* MgrGraphics::GetProjStack()
{
	return &projStack;
}

void MgrGraphics::AttachView(Mtx44 * view)
{
	if (!view) view = &defaultView;
	this->view = view;	
}

void MgrGraphics::SetProjPerspective(float fov, float farVal, float nearVal)
{
	Mtx44 temp;
	temp.SetToPerspective(fov, (double)Application::GetWindowWidth() / Application::GetWindowHeight(), nearVal, farVal);
	projStack.LoadMatrix(temp);
}

void MgrGraphics::SetProjOrtho(float size, float farVal, float nearVal)
{
	Mtx44 temp;
	float scaleSizeX = Application::GetWindowHalfWidth() / size;
	float scaleSizeY = Application::GetWindowHalfHeight() / size;
	temp.SetToOrtho(-scaleSizeX, scaleSizeX, -scaleSizeY, scaleSizeY, nearVal, farVal);
	projStack.LoadMatrix(temp);
}

void MgrGraphics::SetDirLight(bool enabled, Vector3 pos)
{
	if (pos != NULL)
		lightDir.Set(pos.x, pos.y, pos.z, enabled);
}