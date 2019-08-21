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
	MeshBuilder::GeneratePlane("text", 16, 16);
	MeshBuilder::GenerateQuad("quad");	
	MeshBuilder::GeneratePlane("plane", 1, 8);
}

void MgrResource::LoadMaterials()
{
	//Load your materials in here
	new Material("default");
	new Material("fbo");

	Material* temp;

	temp = new Material("placeholder");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/placeholder.tga");

	temp = new Material("font");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/myfont.tga");

	temp = new Material("anim");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/anim2.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/attackL.tga");
	temp->maps[Material::COLOR3] = Resource::LoadTGA("tga/attackR.tga");
	temp->maps[Material::COLOR4] = Resource::LoadTGA("tga/fall.tga");
	temp->maps[Material::COLOR5] = Resource::LoadTGA("tga/jump.tga");
	temp->maps[Material::COLOR6] = Resource::LoadTGA("tga/moveL.tga");
	temp->maps[Material::COLOR7] = Resource::LoadTGA("tga/moveR.tga");
	temp->maps[Material::COLOR8] = Resource::LoadTGA("tga/moveL.tga");
	temp->maps[Material::COLOR9] = Resource::LoadTGA("tga/moveR.tga");
	temp->maps[Material::COLOR10] = Resource::LoadTGA("tga/jump.tga");
	temp->maps[Material::COLOR11] = Resource::LoadTGA("tga/fall.tga");
	temp->maps[Material::COLOR12] = Resource::LoadTGA("tga/fall.tga");

	temp = new Material("background");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/bg_mt.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/bg_snow.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/bg_des.tga");

	temp = new Material("sattest");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/sattest.tga");

	temp = new Material("title");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/title.tga");

	temp = new Material("wasd");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/wasd.tga");

	temp = new Material("sky");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/sky.tga");
}