#include "MgrGraphics.h"
#include "../Utility/WinDebug.h"
#include "../Utility/Input/ControllerKeyboard.h"
#include "../Utility/Graphics/Mesh.h"
#include "../Utility/Graphics/Material.h"
#include "../Node/Components/Renderable.h"

void MgrGraphics::Start()
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

	Debug::Log("Loading shaders...");	
	shaderPrograms[DEFAULT] = Resource::LoadShaders("Shader/default.vert", "Shader/unlit.frag");
	shaderPrograms[SIMPLE] = Resource::LoadShaders("Shader/simple.vert", "Shader/simple.frag");
	
	Node::Start();
}
void MgrGraphics::Update(double dt)
{
	glStencilMask(0xFF); //enable writing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (ControllerKeyboard::Instance()->IsKeyPressed('0'))	
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	if (ControllerKeyboard::Instance()->IsKeyPressed('1'))	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	
	UseShader(DEFAULT);

	Node::Update(dt);
}

void MgrGraphics::End()
{
	Node::End();
}

void MgrGraphics::UseShader(MgrGraphics::SHADER shader)
{
	currShader = shader;
	glUseProgram(shaderPrograms[currShader]);
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

unsigned MgrGraphics::GetUniLoc(std::string uniform, MgrGraphics::SHADER shader)
{
	if (shader != CURRENT && currShader != shader)
		UseShader(shader);
	//return glGetUniformLocation(shader == CURRENT ? shaderPrograms[currShader] : shaderPrograms[shader], uniform.c_str());
	if (cachedUniforms[shader].count(uniform) <= 0)
		cachedUniforms[shader][uniform] = glGetUniformLocation(shader == CURRENT ? shaderPrograms[currShader] : shaderPrograms[shader], uniform.c_str());
	return cachedUniforms[shader][uniform];
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