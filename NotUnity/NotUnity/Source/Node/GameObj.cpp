#include "GameObj.h"

#include "Components/Transform.h"

void GameObj::Start()
{
	comps = AddChild<Components>();
	scripts = AddChild<Scripts>();
	Node::Start();
	t = comps->GetChild<Transform>();
	if (!t) Debug::LogError("Cant find transform!");
}

void GameObj::Update(double dt)
{
	//update children manually
	comps->Update(dt);
	scripts->Update(dt);
	//Node::Update(dt);
}

void GameObj::End()
{
	Node::End();
}

GameObj* GameObj::Create(std::string filePath)
{
	//attach comps and scripts from json
	return this;
}

GameObj* GameObj::Create(Components::PRESET cPreset, Scripts::PRESET sPreset)
{
	this->Start();
	if (cPreset == Components::JSON || cPreset == Scripts::JSON)
	{
		Debug::LogWarning("Provide filePath to load as json!");
		return this;
	}
	//attach comps and scripts from presets
	comps->LoadPreset(cPreset);
	scripts->LoadPreset(sPreset);
	return this;
}
