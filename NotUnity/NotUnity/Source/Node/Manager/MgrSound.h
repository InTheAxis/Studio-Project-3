#ifndef MGR_SOUND_H
#define MGR_SOUND_H

#include "Manager.h"

#include <string>
#include <map>
#include <irrKlang.h>
#include "../../Utility/Math/Vector3.h"

class MgrSound : public Manager<MgrSound>
{
	friend Singleton<MgrSound>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void RegisterSound(std::string name, std::string filePath, float vol = 1);
	void UnregisterSound(std::string name);

	bool isPlaying(std::string name);
	void PlayASound(std::string name, bool looped = false, bool startPaused = false);
	void PlayASound3D(std::string name, Vector3 soundPos, float minDist, float maxDist, bool looped = false, bool startPaused = false);

	void SetVolume(std::string name, float vol = 1);
	void AttachListener(Vector3* listenerPos, Vector3* listenerView);

protected:
	MgrSound(std::string name = "MgrSound");
	~MgrSound();

	irrklang::ISoundEngine* soundEngine;

	Vector3* listenerPos, *listenerView;
	Vector3 listenerPosDef, listenerViewDef;

	std::map<std::string, std::string> sounds;
	std::map<std::string, float> volume;

	float globalVol;
};
#endif