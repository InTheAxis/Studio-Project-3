#include "SoundEngine.h"
#include <iostream>

using namespace std;

// Constructor
CSoundEngine::CSoundEngine()
	: theSoundEngine(NULL)
{
}

// Destructor
CSoundEngine::~CSoundEngine()
{
	// Clear out the sound map
	soundMap.clear();

	// Delete the sound engine
	if (theSoundEngine)
	{
		delete theSoundEngine;
		theSoundEngine = NULL;
	}
}

// Init this class and it will create the Sound Engine
bool CSoundEngine::Init(void)
{
	// Create the sound engine
	theSoundEngine = createIrrKlangDevice();
	if (!theSoundEngine)
		return false;	// error starting up the sound engine

	return true;
}

// Get the handler to the sound engine
ISoundEngine* CSoundEngine::GetSoundEngine(void)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSoundEngine() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	return theSoundEngine;
}

// Add a sound to this library
void CSoundEngine::AddSound(const std::string& _soundIndex, const std::string& _soundSource)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::AddSound() - The Sound Engine has not been initialised yet." << endl;
			return;
		}
	}

	// Clean up first if there is an existing goodie with the same name
	RemoveSound(_soundIndex);

	// Add the mesh now
	soundMap[_soundIndex] = _soundSource;
}

// Get a sound from this map 
std::string CSoundEngine::GetSound(const std::string& _soundIndex)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSound() - The Sound Engine has not been initialised yet." << endl;
			return std::string();
		}
	}

	if (soundMap.count(_soundIndex) != 0)
	{
		return soundMap[_soundIndex];
	}

	return std::string();
}

// Remove a sound from this map
bool CSoundEngine::RemoveSound(const std::string& _soundIndex)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::RemoveSound() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	std::string aSoundName = GetSound(_soundIndex);
	if (!aSoundName.empty())
	{
		soundMap.erase(_soundIndex);
		return true;
	}
	return false;
}

// Play a sound from this map
void CSoundEngine::PlayASound(const std::string& _soundIndex, float vol)
{
	std::string aSound = GetSound(_soundIndex);
	//if (!theSoundEngine->isCurrentlyPlaying(aSound.c_str()))
	{
		// Play a sound
		theSoundEngine->setSoundVolume(vol);
		theSoundEngine->play2D(aSound.c_str(), false, false);
	}
}

void CSoundEngine::SetListenerPos(Vector3 ListenerPos, Vector3 ListenerView)
{
	theSoundEngine->setListenerPosition(vec3df(ListenerPos.x, ListenerPos.y, ListenerPos.z), vec3df(ListenerView.x, ListenerView.y, ListenerView.z));
}

void CSoundEngine::Play3dSound(const std::string& _soundIndex, Vector3 soundPos, float minDist)
{
	std::string aSound = GetSound(_soundIndex);
	vec3df pos;
	pos.X = soundPos.x;
	pos.Y = soundPos.y;
	pos.Z = soundPos.z;

	if (theSoundEngine->isCurrentlyPlaying(aSound.c_str()))
		return;

	ISound* music = theSoundEngine->play3D(aSound.c_str(), pos, true, false, true);
	music->setMinDistance(minDist);

	music->setPosition(pos);	
}
