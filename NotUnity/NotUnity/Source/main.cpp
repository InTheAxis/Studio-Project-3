#include "WinDebug.h"
#include "GameObject.h"
#include "SceneBase.h"

int main()
{
	SceneTest scene;
	scene.m_goList.emplace_back(new GameObject);
	scene.m_goList.emplace_back(new GameObject);

	scene.m_goList[0]->Transform()->temp = 100;
	scene.m_goList.emplace_back(scene.m_goList[0]->Clone());

	return 0;
}