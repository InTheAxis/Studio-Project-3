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
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/myfont.tga");

	temp = new Material("player");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/player/idle.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/player/walk.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/player/jump.tga");
	temp->maps[Material::COLOR3] = Resource::LoadTGA("tga/player/fall.tga");
	temp->maps[Material::COLOR4] = Resource::LoadTGA("tga/player/attack.tga");
	temp->maps[Material::COLOR5] = Resource::LoadTGA("tga/player/air_attack.tga");
	temp->maps[Material::COLOR6] = Resource::LoadTGA("tga/player/hit.tga");
	temp->maps[Material::COLOR7] = Resource::LoadTGA("tga/player/dying.tga");
	temp->maps[Material::COLOR8] = Resource::LoadTGA("tga/player/cheer.tga");

	temp = new Material("sword");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/player/sword_idle.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/player/sword_attack.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/player/sword_air_attack.tga");

	temp = new Material("background");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/env/bg_snow.tga");
	temp->maps[Material::COLOR1] = Resource::LoadTGA("tga/env/bg_snow.tga");
	temp->maps[Material::COLOR2] = Resource::LoadTGA("tga/env/bg_snow.tga");
	temp->maps[Material::COLOR3] = Resource::LoadTGA("tga/env/bg_mt.tga");
	temp->maps[Material::COLOR4] = Resource::LoadTGA("tga/env/bg_mt.tga");
	temp->maps[Material::COLOR5] = Resource::LoadTGA("tga/env/bg_mt.tga");
	temp->maps[Material::COLOR6] = Resource::LoadTGA("tga/env/bg_des.tga");
	temp->maps[Material::COLOR7] = Resource::LoadTGA("tga/env/bg_des.tga");
	temp->maps[Material::COLOR8] = Resource::LoadTGA("tga/env/bg_des.tga");

	temp = new Material("sky");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/env/sky.tga");

	temp = new Material("title");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/title.tga");

	temp = new Material("wasd");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/wasd.tga");

	temp = new Material("lmb");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/lmb.tga");

	temp = new Material("paused");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/paused.tga");

	temp = new Material("enemy1");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/enemy/tomato.tga");

	temp = new Material("enemy2");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/enemy/carrot.tga");

	temp = new Material("enemy3");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/enemy/banana.tga");

	temp = new Material("enemy4");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/enemy/kiwi.tga");

	temp = new Material("enemy5");
	temp->maps[Material::COLOR0] = Resource::LoadTGA("tga/enemy/blueberry.tga");

	temp = new Material("redbar");
	temp-> maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/redbar.tga");

	temp = new Material("greenbar");
	temp-> maps[Material::COLOR0] = Resource::LoadTGA("tga/ui/greenbar.tga");
}


void MgrResource::LoadSounds()
{
	MgrSound* mgS = MgrSound::Instance();	
	mgS->RegisterSound("bgm", "wav/foggy-forest.wav");	
}
