#include "MgrResource.h"

#include "../Utility/Graphics/MeshBuilder.h"
#include "../Utility/Graphics/Material.h"

void MgrResource::Start()
{
	LoadMeshes();
	LoadMaterials();
	Node::Start();
}

void MgrResource::Update(double dt)
{	
	Node::Update(dt);
}

void MgrResource::End()
{
	Node::End();
}

void MgrResource::LoadMeshes()
{
	//Load your meshes in here

	MeshBuilder::GenerateQuad("tri");
}

void MgrResource::LoadMaterials()
{
	//Load your materials in here

	new Material("placeholder");
}
