#ifndef SCENE_BASE_H
#define SCENE_BASE_H

//TODO
/********************************************
  EXAMPLE USAGE

********************************************/

class SceneBase
{
public:
	inline std::string Log(std::string sceneName = "")
	{		
		std::stringstream ss;
		ss << "Logging GameObjects in Scene" << sceneName << ":\n";
		ss << "  " << "<Name>: <Address>\n";
		for (auto go : m_goList)
		{			
			ss << "   " << go->GetName() << ": " << go << "\n";
		}
		
		return ss.str();
	}

	virtual void Init() = 0;

protected:
	std::vector<GameObject*> m_goList;
};

//test scene
class SceneExample : public SceneBase
{
public:
	SceneExample() {}
	~SceneExample() {}

	virtual void Init()
	{
		m_goList = 
		{
			new GameObject("test"),
			new GameObject("test2"),		
		};

		m_goList[0]->AddComponent<ScriptTest>();
		m_goList[1]->AddComponent<ScriptTest2>();		
	}
};

#endif
