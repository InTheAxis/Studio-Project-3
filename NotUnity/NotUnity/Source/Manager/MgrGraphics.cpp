#include "MgrGraphics.h"
#include <iostream>

void MgrGraphics::Start()
{
	Node::Start();
}

void MgrGraphics::Update(double dt)
{
	Node::Update(dt);
	std::cout << "update graphics\n";
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
