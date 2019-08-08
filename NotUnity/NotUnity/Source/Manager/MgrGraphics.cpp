#include "MgrGraphics.h"
#include "../Utility/WinDebug.h"

void MgrGraphics::Start()
{
	Node::Start();
	Debug::Log("Loading shaders...");
}

void MgrGraphics::Update(double dt)
{
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
