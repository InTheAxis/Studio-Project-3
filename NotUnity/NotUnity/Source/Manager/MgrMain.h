#ifndef MGR_MAIN_H
#define MGR_MAIN_H

#include "Manager.h"

#include "MgrScene.h"
#include "MgrGraphics.h"

class MgrMain : public Manager<MgrMain>
{
	friend Singleton<MgrMain>;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	
protected:
	MgrMain(std::string name = "MgrMain") : Manager<MgrMain>(name) {}
	~MgrMain() {}

};
#endif