#pragma once
class SceneBase
{
protected:
public:
	std::vector<GameObject*> m_goList;
};


//test scene
class SceneTest : public SceneBase
{
public:
	SceneTest() {}
	~SceneTest() {}
};
