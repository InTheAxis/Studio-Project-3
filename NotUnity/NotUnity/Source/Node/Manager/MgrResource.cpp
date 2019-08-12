#include "MgrResource.h"

#include "../../Utility/Graphics/MeshBuilder.h"
#include "../../Utility/Graphics/Material.h"

MgrResource::MgrResource(std::string name) : Manager<MgrResource>(name)
{
}

MgrResource::~MgrResource()
{
}

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
	MeshBuilder::GenerateOBJ("placeholder", "obj/placeholder.obj");

	MeshBuilder::GenerateAxes("axes", 10, 10, 10);
	MeshBuilder::GenerateText("text", 16, 16);
	MeshBuilder::GenerateQuad("quad");
}

void MgrResource::LoadMaterials()
{
	//Load your materials in here
	Material* temp;
	new Material("default");

	temp = new Material("placeholder");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/placeholder.tga");

	temp = new Material("font");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/myfont.tga");
}
