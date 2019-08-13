#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#define NUM_PLAYER 3
#define NUM_ENEMY 10

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "Enemy/Enemy3D.h"
#include "Minimap\Minimap.h"
#include "CameraEffects\CameraEffects.h"
#include "HardwareAbstraction/Keyboard.h"
#include "HardwareAbstraction\Mouse.h"

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class ShaderProgram;
class SceneManager;
class TextEntity;
class CFurniture;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo[NUM_PLAYER];
	GroundEntity* groundEntity;
	FPSCamera camera;
	ostringstream DisplayText;
	TextEntity* textObj[3];
	Light* lights[2];

	CKeyboard* theKeyboard;
	CMouse* theMouse;
	//CMinimap* theMinimap;
	CCameraEffects* theCameraEffects;
	CFurniture* aChair;

	float timeScale;
	int currPlayer;
	bool debugTimeScale = false;
	int score;

	GenericEntity* theCube;
	CEnemy3D* anEnemy3D[NUM_ENEMY];	// This is the CEnemy class for 3D use.

	static SceneText* sInstance; // The pointer to the object that gets registered


	void ChangePlayer(int i);
};

#endif