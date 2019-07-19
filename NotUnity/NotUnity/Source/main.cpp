#include "Utility/WinDebug.h"
#include "Node/NScene.h"
#include "Node/NGameObj.h"

int main()
{
	NScene* scene = new NScene;

	NGameObj* GO = new NGameObj;

	scene->AddChild<NGameObj>();

	NGameObj* temp = scene->GetChild<NGameObj>();
	return 0;
}