#include "MgrSound.h"

#include "../Components/Collider.h"

MgrSound::MgrSound(std::string name) 
	: Manager<MgrSound>(name)
	, listenerPos(nullptr)
	, listenerView(nullptr)
	, listenerPosDef(0,0,0)
	, listenerViewDef(0,0,-1)
{
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine)
		Debug::LogWarning("Sound Engine not initialised");
}

MgrSound::~MgrSound()
{
	sounds.clear();
	if (soundEngine)
	{
		delete soundEngine;
		soundEngine = nullptr;
	}
}

void MgrSound::Start()
{			
	Node::Start();
}

void MgrSound::Update(double dt)
{
	if (!listenerPos) listenerPos = &listenerPosDef;
	if (!listenerView) listenerView = &listenerViewDef;
	soundEngine->setListenerPosition(
		irrklang::vec3df(listenerPos->x, listenerPos->y, listenerPos->z),
		irrklang::vec3df(listenerView->x, listenerView->y, listenerView->z));
	Node::Update(dt);
}

void MgrSound::End()
{
	Node::End();
}

void MgrSound::RegisterSound(std::string name, std::string filePath, float vol)
{
	if (sounds.count(name) > 0)
		return Debug::Log("Sound " + name + " already exists!");
	sounds[name] = "Resources/" + filePath;
	SetVolume(name);
}

void MgrSound::UnregisterSound(std::string name)
{
	if (sounds.count(name) > 0)
		sounds.erase(name);
	if (volume.count(name) > 0)
		volume.erase(name);
}

bool MgrSound::isPlaying(std::string name) 
{
	if (sounds.count(name) <= 0)
		return false;
	return soundEngine->isCurrentlyPlaying(sounds[name].c_str());
}

void MgrSound::PlayASound(std::string name, bool looped, bool startPaused)
{
	if (sounds.count(name) <= 0)
		return Debug::LogWarning("Sound " + name + " not found");

	irrklang::ISound* snd =	soundEngine->play2D(sounds[name].c_str(), looped, startPaused, true);
	snd->setVolume(volume[name]);
}

void MgrSound::PlayASound3D(std::string name, Vector3 soundPos, float minDist, float maxDist, bool looped, bool startPaused)
{
	if (sounds.count(name) <= 0)
		return Debug::LogWarning("Sound " + name + " not found");

	irrklang::vec3df pos;
	pos.X = soundPos.x;
	pos.Y = soundPos.y;
	pos.Z = soundPos.z;

	irrklang::ISound* snd = soundEngine->play3D(sounds[name].c_str(), pos, looped, startPaused, true);
	snd->setVolume(volume[name]);
	snd->setMinDistance(minDist);
	snd->setMaxDistance(maxDist);
	snd->setPosition(pos);	
}

void MgrSound::SetVolume(std::string name, float vol)
{
	if (volume.count(name) <= 0)
		volume[name] = Math::Clamp(vol, 0.f, 1.f);
}

void MgrSound::AttachListener(Vector3* listenerPos, Vector3* listenerView)
{
	this->listenerPos = listenerPos;
	this->listenerView = listenerView;
}