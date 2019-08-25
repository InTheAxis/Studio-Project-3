#include "MgrResource.h"

#include "../../Utility/Graphics/MeshBuilder.h"
#include "../../Utility/Graphics/Material.h"
#include "MgrSound.h"

MgrResource::MgrResource(std::string name) : Manager<MgrResource>(name)
{
}

MgrResource::~MgrResource()
{
}

void MgrResource::Start()
{
	Debug::Log("Loading meshes...");
	LoadMeshes();
	Debug::Log("Loading materials...");
	LoadMaterials();
	Debug::Log("Loading sounds...");
	LoadSounds();
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

	temp = new Material("player");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR3] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR4] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR5] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR6] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR7] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR8] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR9] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR10] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR11] = Resource::LoadTGA("tga/idle_right.tga");
	temp->maps[Material::COLOR12] = Resource::LoadTGA("tga/idle_right.tga");

	temp = new Material("background");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/bg_snow.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/bg_snow.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/bg_snow.tga");
	temp->maps[Material::COLOR3] = Resource::LoadTGA("tga/bg_mt.tga");
	temp->maps[Material::COLOR4] = Resource::LoadTGA("tga/bg_mt.tga");
	temp->maps[Material::COLOR5] = Resource::LoadTGA("tga/bg_mt.tga");
	temp->maps[Material::COLOR6] = Resource::LoadTGA("tga/bg_des.tga");
	temp->maps[Material::COLOR7] = Resource::LoadTGA("tga/bg_des.tga");
	temp->maps[Material::COLOR8] = Resource::LoadTGA("tga/bg_des.tga");

	temp = new Material("sattest");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/sattest.tga");

	temp = new Material("title");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/title.tga");

	temp = new Material("wasd");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/wasd.tga");

	temp = new Material("lmb");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/lmb.tga");

	temp = new Material("sky");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/sky.tga");

	temp = new Material("enemy");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/tomato.tga");

	temp = new Material("redbar");
	temp-> maps[Material::COLOR0] = Resource::LoadTGA("tga/redbar.tga");

	temp = new Material("greenbar");
	temp-> maps[Material::COLOR0] = Resource::LoadTGA("tga/greenbar.tga");
}


void MgrResource::LoadSounds()
{
	MgrSound* mgS = MgrSound::Instance();	
	mgS->RegisterSound("bgm", "wav/foggy-forest.wav");	
}
