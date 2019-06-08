#include "WinDebug.h"
#include "GameObject.h"
#include "SceneBase.h"

int main()
{
	SceneBase* scene = new SceneExample;

	scene->Init();

	Debug::Log(scene->Log("Test"));
	Debug::Log(MgrGObject::Instance().Log("GObject"));

	return 0;
}