#ifndef MGR_MAIN_H
#define MGR_MAIN_H

#include <vector>

#include "Manager.h"

class MgrMain : public Manager<MgrMain>
{
	friend Singleton<MgrMain>;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void SetTimeScale(float timeScale);
protected:
	MgrMain(std::string name = "MgrMain");
	~MgrMain();
	
	std::vector<Node*> allManagers;
	float timeScale;
};
#endif