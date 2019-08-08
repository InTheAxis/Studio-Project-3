#include "MgrGraphics.h"
#include "../Utility/WinDebug.h"
#include "../Utility/Input/ControllerKeyboard.h"
#include "../Node/Renderable.h"

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
	shaderPrograms[DEFAULT] = Resource::LoadShaders("Shader/default.vert", "Shader/simple.frag");
	
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

void MgrGraphics::CacheMesh(Mesh * mesh)
{
	cachedMeshes[mesh->name] = mesh;
}

Mesh* MgrGraphics::GetCachedMesh(std::string name)
{
	return cachedMeshes[name];
}

void MgrGraphics::UseShader(MgrGraphics::SHADER shader)
{
	currShader = shader;
	glUseProgram(shaderPrograms[currShader]);
}
